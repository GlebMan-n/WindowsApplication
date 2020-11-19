// WMIO2.h

#define WMIO2_API __declspec(dllexport)

extern "C" WMIO2_API bool ModeOpen(UINT uiMode);
extern "C" WMIO2_API bool ModeClose();
extern "C" WMIO2_API bool SetDevice(BYTE uiValue);
extern "C" WMIO2_API bool GetDevice(PBYTE puiValue);
extern "C" WMIO2_API bool SetBrightness(BYTE uiValue);
extern "C" WMIO2_API bool GetBrightness(PBYTE puiValue);
extern "C" WMIO2_API bool GetSMBIOSInfo(PCHAR pstName, PCHAR pstValue);


