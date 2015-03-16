#ifdef PAXOS_EXPORTS
#define PAXOS_API __declspec(dllexport)
#else
#define PAXOS_API __declspec(dllimport)
#endif

#pragma warning(disable:4251)