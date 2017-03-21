// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MULTIPRECISIONARITHMETICS_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MULTIPRECISIONARITHMETICS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MULTIPRECISIONARITHMETICS_EXPORTS
#define MULTIPRECISIONARITHMETICS_API __declspec(dllexport)
#else
#define MULTIPRECISIONARITHMETICS_API __declspec(dllimport)
#endif

// This class is exported from the MultiPrecisionArithmetics.dll
class MULTIPRECISIONARITHMETICS_API CMultiPrecisionArithmetics {
public:
	CMultiPrecisionArithmetics(void);
	// TODO: add your methods here.
};

extern MULTIPRECISIONARITHMETICS_API int nMultiPrecisionArithmetics;

MULTIPRECISIONARITHMETICS_API int fnMultiPrecisionArithmetics(void);
