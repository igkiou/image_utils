/*
 * libraw_mex.cpp
 *
 *  Created on: Jan 7, 2014
 *      Author: igkiou
 */

#include <iostream>
#include "raw_mex.h"

namespace canon {

mxArray* getCameraProperty(const EdsCameraRef handle, \
							const CAMERA_PROPERTY property) {

	switch(property) {
	case CAMERA_AEMODE: {
			EdsUInt32 AEMode;
			EdsError errorCode = EdsGetPropertyData(handle,
													kEdsPropID_AEMode, 0,
													sizeof(AEMode),
													&AEMode);
			if (errorCode != EDS_ERR_OK) {
				handleErrorCode(errorCode);
			}
			mxArray *mxarr = mxCreateDoubleScalar(AEModeEdsToDouble(AEMode));
			return mxarr;
		}
		case CAMERA_DRIVEMODE: {
			EdsUInt32 driveMode;
			EdsError errorCode = EdsGetPropertyData(handle,
													kEdsPropID_DriveMode, 0,
													sizeof(driveMode),
													&driveMode);
			if (errorCode != EDS_ERR_OK) {
				handleErrorCode(errorCode);
			}
			mxArray *mxarr = mxCreateDoubleScalar(driveModeEdsToDouble(driveMode));
			return mxarr;
		}
		case CAMERA_IMAGEQUALITY: {
			EdsUInt32 imageQuality;
			EdsError errorCode = EdsGetPropertyData(handle,
													kEdsPropID_ImageQuality, 0,
													sizeof(imageQuality),
													&imageQuality);
			if (errorCode != EDS_ERR_OK) {
				handleErrorCode(errorCode);
			}
			mxArray *mxarr = mxCreateDoubleScalar(imageQualityEdsToDouble(imageQuality));
			return mxarr;
		}
		case CAMERA_ISOSPEED: {
			EdsUInt32 isoSpeed;
			EdsError errorCode = EdsGetPropertyData(handle,
													kEdsPropID_ISOSpeed, 0,
													sizeof(isoSpeed),
													&isoSpeed);
			if (errorCode != EDS_ERR_OK) {
				handleErrorCode(errorCode);
			}
			mxArray *mxarr = mxCreateDoubleScalar(isoSpeedEdsToDouble(isoSpeed));
			return mxarr;
		}
		case CAMERA_APERTUREVALUE: {
			EdsUInt32 apertureValue;
			EdsError errorCode = EdsGetPropertyData(handle,
													kEdsPropID_Av, 0,
													sizeof(apertureValue),
													&apertureValue);
			if (errorCode != EDS_ERR_OK) {
				handleErrorCode(errorCode);
			}
			mxArray *mxarr = mxCreateDoubleScalar(apertureValueEdsToDouble(apertureValue));
			return mxarr;
		}
		case CAMERA_SHUTTERSPEED: {
			EdsUInt32 shutterSpeed;
			EdsError errorCode = EdsGetPropertyData(handle,
													kEdsPropID_Tv, 0,
													sizeof(shutterSpeed),
													&shutterSpeed);
			if (errorCode != EDS_ERR_OK) {
				handleErrorCode(errorCode);
			}
			mxArray *mxarr = mxCreateDoubleScalar(shutterSpeedEdsToDouble(shutterSpeed));
			return mxarr;
		}
		case CAMERA_EVFOUTPUTDEVICE: {
			EdsUInt32 evfOutputDevice;
			EdsError errorCode = EdsGetPropertyData(handle,
													kEdsPropID_Evf_OutputDevice, 0,
													sizeof(evfOutputDevice),
													&evfOutputDevice);
			if (errorCode != EDS_ERR_OK) {
				handleErrorCode(errorCode);
			}
			mxArray *mxarr = mxCreateDoubleScalar(evfOutputDeviceEdsToDouble(evfOutputDevice));
			return mxarr;
		}
		case CAMERA_EVFMODE: {
			EdsUInt32 evfMode;
			EdsError errorCode = EdsGetPropertyData(handle,
													kEdsPropID_Evf_Mode, 0,
													sizeof(evfMode),
													&evfMode);
			if (errorCode != EDS_ERR_OK) {
				handleErrorCode(errorCode);
			}
			mxArray *mxarr = mxCreateDoubleScalar(evfModeEdsToDouble(evfMode));
			return mxarr;
		}
		case CAMERA_SAVETO: {
			EdsUInt32 saveTo;
			EdsError errorCode = EdsGetPropertyData(handle,
													kEdsPropID_SaveTo, 0,
													sizeof(saveTo),
													&saveTo);
			if (errorCode != EDS_ERR_OK) {
				handleErrorCode(errorCode);
			}
			mxArray *mxarr = mxCreateDoubleScalar(saveToEdsToDouble(saveTo));
			return mxarr;
		}
		case CAMERA_AVAILABLESHOTS: {
			EdsUInt32 availableShots;
			EdsError errorCode = EdsGetPropertyData(handle,
													kEdsPropID_AvailableShots, 0,
													sizeof(availableShots),
													&availableShots);
			if (errorCode != EDS_ERR_OK) {
				handleErrorCode(errorCode);
			}
			mxArray *mxarr = mxCreateDoubleScalar(availableShotsEdsToDouble(availableShots));
			return mxarr;
		}
		default: {
			char propertyName[canon_MAX_STRING_LENGTH];
			cameraPropertyToString(property, propertyName);
			mexErrMsgIdAndTxt(ERROR_ID, "Unknown or unsupported camera property: %s.", propertyName);
			return NULL;
		}
	}
}

mxArray* getCameraProperty(const EdsCameraRef handle) {

	unsigned numProperties = (unsigned) CAMERA_PROPERTY_LENGTH;
	mxArray *mxstruct;
	char **propertyNames = (char **) \
			mxMalloc(numProperties * sizeof(*propertyNames));
	for (unsigned iterProperty = 0; iterProperty < numProperties; ++iterProperty) {
		propertyNames[iterProperty] = (char *) mxMalloc(canon_MAX_STRING_LENGTH * sizeof(char));
		cameraPropertyToString((CAMERA_PROPERTY) iterProperty, propertyNames[iterProperty]);
	}
	mxstruct = mxCreateStructMatrix(1, 1, numProperties, (const char **) propertyNames);
	for (unsigned iterProperty = 0; iterProperty < numProperties; ++iterProperty) {
		mxSetFieldByNumber(mxstruct, 0, iterProperty, getCameraProperty(handle, (CAMERA_PROPERTY) iterProperty));
	}
	for (unsigned iterProperty = 0; iterProperty < numProperties; ++iterProperty) {
		mxFree((void *) propertyNames[iterProperty]);
	}
	mxFree((void *) propertyNames);
	return mxstruct;
}

void setCameraProperty(const EdsCameraRef handle, \
							const CAMERA_PROPERTY property, \
							const mxArray* mxarr) {

	switch(property) {
		case CAMERA_AEMODE: {
			EdsUInt32 AEMode = AEModeDoubleToEds(mxGetScalar(mxarr));
			EdsError errorCode = EdsSetPropertyData(handle,
													kEdsPropID_AEMode, 0,
													sizeof(AEMode),
													&AEMode);
			if (errorCode != EDS_ERR_OK) {
				handleErrorCode(errorCode);
			}
			break;
		}
		case CAMERA_DRIVEMODE: {
			EdsUInt32 driveMode = driveModeDoubleToEds(mxGetScalar(mxarr));
			EdsError errorCode = EdsSetPropertyData(handle,
													kEdsPropID_DriveMode, 0,
													sizeof(driveMode),
													&driveMode);
			if (errorCode != EDS_ERR_OK) {
				handleErrorCode(errorCode);
			}
			break;
		}
		case CAMERA_IMAGEQUALITY: {
			EdsUInt32 imageQuality = imageQualityDoubleToEds(mxGetScalar(mxarr));
			EdsError errorCode = EdsSetPropertyData(handle,
													kEdsPropID_ImageQuality, 0,
													sizeof(imageQuality),
													&imageQuality);
			if (errorCode != EDS_ERR_OK) {
				handleErrorCode(errorCode);
			}
			break;
		}
		case CAMERA_ISOSPEED: {
			EdsUInt32 isoSpeed = isoSpeedDoubleToEds(mxGetScalar(mxarr));
			EdsError errorCode = EdsSetPropertyData(handle,
													kEdsPropID_ISOSpeed, 0,
													sizeof(isoSpeed),
													&isoSpeed);
			if (errorCode != EDS_ERR_OK) {
				handleErrorCode(errorCode);
			}
			break;
		}
		case CAMERA_APERTUREVALUE: {
			EdsUInt32 apertureValue = apertureValueDoubleToEds(mxGetScalar(mxarr));
			EdsError errorCode = EdsSetPropertyData(handle,
													kEdsPropID_Av, 0,
													sizeof(apertureValue),
													&apertureValue);
			if (errorCode != EDS_ERR_OK) {
				handleErrorCode(errorCode);
			}
			break;
		}
		case CAMERA_SHUTTERSPEED: {
			EdsUInt32 shutterSpeed = shutterSpeedDoubleToEds(mxGetScalar(mxarr));
			EdsError errorCode = EdsSetPropertyData(handle,
													kEdsPropID_Tv, 0,
													sizeof(shutterSpeed),
													&shutterSpeed);
			if (errorCode != EDS_ERR_OK) {
				handleErrorCode(errorCode);
			}
			break;
		}
		case CAMERA_EVFOUTPUTDEVICE: {
			EdsUInt32 evfOutputDevice = evfOutputDeviceDoubleToEds(mxGetScalar(mxarr));
			EdsError errorCode = EdsSetPropertyData(handle,
													kEdsPropID_Evf_OutputDevice, 0,
													sizeof(evfOutputDevice),
													&evfOutputDevice);
			if (errorCode != EDS_ERR_OK) {
				handleErrorCode(errorCode);
			}
			break;
		}
		case CAMERA_EVFMODE: {
			EdsUInt32 evfMode = evfModeDoubleToEds(mxGetScalar(mxarr));
			EdsError errorCode = EdsSetPropertyData(handle,
													kEdsPropID_Evf_Mode, 0,
													sizeof(evfMode),
													&evfMode);
			if (errorCode != EDS_ERR_OK) {
				handleErrorCode(errorCode);
			}
			break;
		}
		case CAMERA_SAVETO: {
			EdsUInt32 saveTo = saveToDoubleToEds(mxGetScalar(mxarr));
			EdsError errorCode = EdsSetPropertyData(handle,
													kEdsPropID_Evf_Mode, 0,
													sizeof(saveTo),
													&saveTo);
			if (errorCode != EDS_ERR_OK) {
				handleErrorCode(errorCode);
			}
			break;
		}
		default:
		{
			char propertyName[canon_MAX_STRING_LENGTH];
			cameraPropertyToString(property, propertyName);
			mexErrMsgIdAndTxt(ERROR_ID, "Unknown or unsupported camera property: %s.", propertyName);
		}
	}
}

void setCameraProperty(const EdsCameraRef handle, const mxArray* mxstruct) {

	if (mxIsStruct(mxstruct)) {
		unsigned numProperties = mxGetNumberOfFields(mxstruct);
		for (unsigned iterProperty = 0; iterProperty < numProperties; ++iterProperty) {
			setCameraProperty(handle, stringToCameraProperty(mxGetFieldNameByNumber(mxstruct, iterProperty)), \
							mxGetFieldByNumber(mxstruct, 0, iterProperty));
		}
	} else if (!mxIsEmpty(mxstruct)) {
		mexErrMsgIdAndTxt(ERROR_ID, "For setting multiple attributes, a struct array must be provided.");
	}
}

int getNumberOfImages_sub(EdsDirectoryItemRef directoryItem) {

	EdsUInt32 numChildren;
	EdsGetChildCount(directoryItem, &numChildren);
	EdsDirectoryItemInfo directoryItemInfo;
	EdsGetDirectoryItemInfo(directoryItem, &directoryItemInfo);
	int numFiles = 0;
	EdsDirectoryItemRef childItem;
	EdsDirectoryItemInfo childItemInfo;
	for (int iter = 0; iter < numChildren; ++iter) {
		EdsGetChildAtIndex(directoryItem, iter, &childItem);
		EdsGetDirectoryItemInfo(childItem, &childItemInfo);
		if (childItemInfo.isFolder == true) {
			numFiles += getNumberOfImages_sub(childItem);
		} else {
			++numFiles;
		}
		EdsRelease(childItem);
	}
	return numFiles;
}

int getNumberOfImages(EdsCameraRef handle) {
	EdsVolumeRef volume;
	EdsGetChildAtIndex(handle, 0, &volume);
	EdsDirectoryItemRef dcim;
	EdsGetChildAtIndex(volume, 0 , &dcim);
	int numFiles = getNumberOfImages_sub(dcim);
	EdsRelease(dcim);
	EdsRelease(volume);
	return numFiles;
}

void initializeSDK() {
	EdsError err = EdsInitializeSDK();
	if (err != EDS_ERR_OK) {
		handleErrorCode(err);
	}
}

void terminateSDK() {
	EdsError err = EdsTerminateSDK();
	if (err != EDS_ERR_OK) {
		handleErrorCode(err);
	}
}

unsigned queryCamera() {

	EdsCameraListRef cameraList = NULL;
	EdsError err = EdsGetCameraList(&cameraList);
	if (err != EDS_ERR_OK) {
	}

	EdsUInt32 numCameras = 0;
	err = EdsGetChildCount(cameraList, &numCameras);
	if (err == EDS_ERR_OK) {
		handleErrorCode(err);
	}

	// Release camera list
	if (cameraList != NULL) {
		EdsRelease(cameraList);
		cameraList = NULL;
	}

	return (unsigned) numCameras;
}

EdsError EDSCALLBACK handleObjectEvent(EdsObjectEvent event, EdsBaseRef object,
									EdsVoid * context) {
//	// do something
//	switch(event) {
//		case kEdsObjectEvent_DirItemRequestTransfer:
//			downloadImage(object);
//			break;
//		default:
//			break;
//	}
//
//	// Object must be released
//	if (object) {
//		EdsRelease(object);
//	}
	return EDS_ERR_OK;
}

EdsError EDSCALLBACK handlePropertyEvent(EdsPropertyEvent event, EdsPropertyID property,
										EdsUInt32 inParam, EdsVoid * context) {
	return EDS_ERR_OK;
}

EdsError EDSCALLBACK handleStateEvent(EdsStateEvent event, EdsUInt32 parameter,
									EdsVoid * context) {
	return EDS_ERR_OK;
}


EdsCameraRef openCamera() {
	EdsCameraListRef cameraList = NULL;
	// Get camera list
	EdsError err = EdsGetCameraList(&cameraList);
	if (err != EDS_ERR_OK) {
		handleErrorCode(err);
	}

	EdsUInt32 numCameras = 0;
	// Get number of cameras
	err = EdsGetChildCount(cameraList, &numCameras);
	if (err == EDS_ERR_OK) {
		handleErrorCode(err);
	}

	if (numCameras > 1) {
		mexWarnMsgTxt("Warning: more than one available cameras found. Multiple cameras are not well supported.");
	} else if (numCameras <= 0) {
		mexErrMsgIdAndTxt(ERROR_ID, "No available camera found.");
		return NULL;
	}

	EdsCameraRef handle;
	// Get first camera retrieved
	err = EdsGetChildAtIndex(cameraList , 0 , &handle);
	if (err != EDS_ERR_OK) {
		handleErrorCode(err);
	}

	// Release camera list
	if (cameraList != NULL) {
		EdsRelease(cameraList);
		cameraList = NULL;
	}

	// Set object event handler
	err = EdsSetObjectEventHandler(handle, kEdsObjectEvent_All, handleObjectEvent, NULL);
	if (err != EDS_ERR_OK) {
		handleErrorCode(err);
	}

	// Set property event handler
	err = EdsSetPropertyEventHandler(handle, kEdsPropertyEvent_All, handlePropertyEvent, NULL);
	if (err != EDS_ERR_OK) {
		handleErrorCode(err);
	}

	// Set camera state event handler
	err = EdsSetCameraStateEventHandler(handle, kEdsStateEvent_All, handleStateEvent, NULL);
	if (err != EDS_ERR_OK) {
		handleErrorCode(err);
	}

	// Open session with camera
	err = EdsOpenSession(handle);
	if (err != EDS_ERR_OK) {
		handleErrorCode(err);
	}

	return handle;
}

void closeCamera(EdsCameraRef handle) {
	EdsError err = EdsCloseSession(handle);
	if (err != EDS_ERR_OK) {
		handleErrorCode(err);
	}

	// Release camera
	if (handle != NULL) {
		EdsRelease(handle);
	}
}

void snap(const EdsCameraRef handle) {
	EdsError errorCode = EdsSendCommand(handle, kEdsCameraCommand_TakePicture, 0);
	if (errorCode != EDS_ERR_OK) {
		handleErrorCode(errorCode);
	}
}


void capture(const EdsCameraRef handle, \
			const double isoSpeedDouble, const double apertureValueDouble, \
			const double shutterSpeedDouble) {

	EdsError errorCode = EDS_ERR_OK;

	EdsUInt32 isoSpeed = isoSpeedDoubleToEds(isoSpeedDouble);
	errorCode = EdsSetPropertyData(handle, kEdsPropID_ISOSpeed, 0,
								sizeof(isoSpeed), &isoSpeed);
	if (errorCode != EDS_ERR_OK) {
		handleErrorCode(errorCode);
	}

	EdsUInt32 apertureValue = apertureValueDoubleToEds(apertureValueDouble);
	errorCode = EdsSetPropertyData(handle, kEdsPropID_Av, 0,
								sizeof(apertureValue), &apertureValue);
	if (errorCode != EDS_ERR_OK) {
		handleErrorCode(errorCode);
	}

	EdsUInt32 shutterSpeed = shutterSpeedDoubleToEds(shutterSpeedDouble);
	errorCode = EdsSetPropertyData(handle, kEdsPropID_Tv, 0,
								sizeof(shutterSpeed), &shutterSpeed);
	if (errorCode != EDS_ERR_OK) {
		handleErrorCode(errorCode);
	}

	snap(handle);
}


void captureBracket(const EdsCameraRef handle, \
			const double *isoSpeeds, const double *apertureValues, \
			const double *shutterSpeeds, const unsigned bracketLength) {
	for (unsigned bracketIter = 0; bracketIter < bracketLength; ++bracketIter) {
		capture(handle, isoSpeeds[bracketIter], apertureValues[bracketIter],
				shutterSpeeds[bracketIter]);
	}
}

void pressShutter(const EdsCameraRef handle) {
	EdsError errorCode = EdsSendCommand(handle,
										kEdsCameraCommand_PressShutterButton,
										kEdsCameraCommand_ShutterButton_Completely_NonAF);
	if (errorCode != EDS_ERR_OK) {
		handleErrorCode(errorCode);
	}
}

void releaseShutter(const EdsCameraRef handle) {
	EdsError errorCode = EdsSendCommand(handle,
										kEdsCameraCommand_PressShutterButton,
										kEdsCameraCommand_ShutterButton_OFF);
	if (errorCode != EDS_ERR_OK) {
		handleErrorCode(errorCode);
	}
}


}	/* namespace canon */
