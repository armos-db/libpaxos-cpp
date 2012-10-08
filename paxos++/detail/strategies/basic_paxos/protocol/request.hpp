/*!
  Copyright (c) 2012, Leon Mergen, all rights reserved.
 */

#ifndef LIBPAXOS_CPP_DETAIL_CLIENT_PROTOCOL_REQUEST_HPP
#define LIBPAXOS_CPP_DETAIL_CLIENT_PROTOCOL_REQUEST_HPP

#include <boost/asio/ip/tcp.hpp>

#include "../../../tcp_connection.hpp"

namespace paxos { namespace detail { namespace quorum { 
class quorum;
}; }; };

namespace paxos { namespace detail { 
class command;
class paxos_state;
}; };

namespace paxos { namespace detail { namespace strategies { namespace basic_paxos { namespace protocol {

/*!
  \brief Defines protocol used when a leader must propagate a request throughout the quorum
 */
class request
{
private:
   enum response
   {
      response_none,
      response_ack,
      response_reject
   };

   struct state
   {
      std::map <boost::asio::ip::tcp::endpoint, enum response>                   accepted;
      std::map <boost::asio::ip::tcp::endpoint, std::string>                     responses;
      std::map <boost::asio::ip::tcp::endpoint, detail::tcp_connection::pointer> connections;
   };
   
public:

   /*!
     \brief Received by leader from client that initiates a request
    */
   static void
   step1 (      
      tcp_connection::pointer   client_connection,
      detail::command const &   command,
      detail::quorum::quorum &  quorum,
      detail::paxos_state &     state);


   /*!
     \brief Received by follower when leader wants to prepare a request
    */
   static void
   step3 (      
      tcp_connection::pointer   leader_connection,
      detail::command const &   command,
      detail::quorum::quorum &  quorum,
      detail::paxos_state &     state);


   /*!
     \brief Received by follower when leader wants to process a request

     This request is received by a follower when a leader has received an 'ACK' for a certain
     proposal id from all followers, and is now telling all followers to actually process a 
     command.
    */
   static void
   step6 (      
      tcp_connection::pointer   leader_connection,
      detail::command const &   command,
      detail::quorum::quorum &  quorum,
      detail::paxos_state &     state);

private:

   /*!
     \brief Sends a 'prepare' to a specific server
    */
   static void
   step2 (
      tcp_connection::pointer                   client_connection,
      boost::asio::ip::tcp::endpoint const &    leader_endpoint,
      boost::asio::ip::tcp::endpoint const &    follower_endpoint,
      tcp_connection::pointer                   follower_connection,
      uint64_t                                  proposal_id,
      std::string const &                       byte_array,
      boost::shared_ptr <struct state>          state);


   /*!
     \brief Received by leader as a response to a 'prepare' command
    */
   static void
   step4 (
      tcp_connection::pointer                   client_connection,
      boost::asio::ip::tcp::endpoint const &    leader_endpoint,
      boost::asio::ip::tcp::endpoint const &    follower_endpoint,
      tcp_connection::pointer                   follower_connection,
      uint64_t                                  proposal_id,
      std::string                               byte_array,
      detail::command const &                   command,
      boost::shared_ptr <struct state>          state);

   /*!
     \brief Sends a 'accept' from leader to a specific follower
    */
   static void
   step5 (
      tcp_connection::pointer                   client_connection,
      boost::asio::ip::tcp::endpoint const &    leader_endpoint,
      boost::asio::ip::tcp::endpoint const &    follower_endpoint,
      tcp_connection::pointer                   follower_connection,
      uint64_t                                  proposal_id,
      std::string const &                       byte_array,
      boost::shared_ptr <struct state>          state);


   /*!
     \brief Received by leader as a response to a 'accept' command
    */
   static void
   step7 (
      tcp_connection::pointer                   client_connection,
      boost::asio::ip::tcp::endpoint const &    leader_endpoint,
      boost::asio::ip::tcp::endpoint const &    follower_endpoint,
      tcp_connection::pointer                   follower_connection,
      detail::command const &                   command,
      boost::shared_ptr <struct state>          state);

private:   

};

}; }; }; }; };

#endif //! LIBPAXOS_CPP_DETAIL_CLIENT_PROTOCOL_REQUEST_HPP
