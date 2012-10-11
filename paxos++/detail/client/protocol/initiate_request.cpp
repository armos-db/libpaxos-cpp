#include "../../util/debug.hpp"
#include "../../command.hpp"
#include "../../parser.hpp"
#include "../../quorum/quorum.hpp"
#include "../../tcp_connection.hpp"

#include "initiate_request.hpp"

namespace paxos { namespace detail { namespace client { namespace protocol {

/*! static */ void
initiate_request::step1 (      
   std::string const &       byte_array,
   detail::quorum::quorum &  quorum,
   callback_type             callback) throw (exception::not_ready)
{
   /*!
     If the quorum doesn't have a leader, we're not ready yet.
   */
   PAXOS_CHECK_THROW (quorum.we_have_a_leader_connection () == false, exception::not_ready ());
   PAXOS_CHECK_THROW (quorum.has_majority (quorum.who_is_our_leader ()) == false, exception::not_ready ());

   PAXOS_DEBUG ("quorum is reading, sending request..");

   /*!
     \bug There is a race condition here: quorum.our_leader_connection () assumes that
          quorum.we_have_a_leader () == true, but another thread in the meantime may have
          adjusted our quorum so that the previous check in the line above no longer holds
          true, and would thus lead to an assertion.
   */

   detail::tcp_connection_ptr connection = quorum.our_leader_connection ();
 
   /*!
     Now that we have our leader's connection, let's send it our command to initiate
     the request. 
   */
   command command;
   command.set_type (command::type_request_initiate);
   command.set_workload (byte_array);
   connection->command_dispatcher ().write (command);

   PAXOS_INFO ("client initiating request!");


   connection->command_dispatcher ().read (
      command,
      [connection,
       callback] (
          detail::command const &   c)
      {
         switch (c.type ())
         {
               case command::type_request_accepted:
                  PAXOS_DEBUG ("received command with workload = " << c.workload () << ", "
                               "now calling callback!");
                  callback (boost::none, c.workload ());
                  break;
                  
               case command::type_request_error:
                  PAXOS_WARN ("request error occured");
                  callback (c.error_code (), c.workload ());
                  break;

               default:
                  PAXOS_UNREACHABLE ();
         };
      });
}


}; }; }; };
