// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the HASHLIBRARY_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// HASHLIBRARY_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef HASHLIBRARY_EXPORTS
#define HASHLIBRARY_API __declspec(dllexport)
#else
#define HASHLIBRARY_API __declspec(dllimport)
#endif

#include<cstdint>
#include<vector>
typedef std::vector<uint8_t> Bytes;

HASHLIBRARY_API Bytes DummyHash(const Bytes &msg);
