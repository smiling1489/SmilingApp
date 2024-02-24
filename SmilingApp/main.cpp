#include <ctime>
#include <Windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include "resource.h"

bool ChangeVolume(double nVolume, bool bScalar)
{
	HRESULT hr = NULL;
	bool decibels = false;
	bool scalar = false;
	double newVolume = nVolume;

	HRESULT HCoInit = CoInitialize(NULL);

	IMMDeviceEnumerator* deviceEnumerator = NULL;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);
	IMMDevice* defaultDevice = NULL;

	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	deviceEnumerator->Release();
	deviceEnumerator = NULL;

	IAudioEndpointVolume* endpointVolume = NULL;
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume),
		CLSCTX_INPROC_SERVER, NULL, (LPVOID*)&endpointVolume);
	defaultDevice->Release();
	defaultDevice = NULL;

	float currentVolume = 0;
	endpointVolume->GetMasterVolumeLevel(&currentVolume);

	hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);

	if (bScalar == false)
		hr = endpointVolume->SetMasterVolumeLevel((float)newVolume, NULL);
	else if (bScalar == true)
		hr = endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL);

	endpointVolume->SetMute(FALSE, NULL);
	endpointVolume->Release();

	CoUninitialize();

	return FALSE;
}

int main()
{
	srand(time(NULL));

	const int IDR_LIST[4] = { IDR_WAVE1, IDR_WAVE2, IDR_WAVE3, IDR_WAVE4 };

	PlaySound(MAKEINTRESOURCE(IDR_LIST[rand() % 4]), GetModuleHandle(NULL), SND_RESOURCE | SND_LOOP | SND_ASYNC);

	while (true)
	{
		ChangeVolume(1.0, true);
	}

	return 0;
}
