#ifndef __VS_ERROR_CONSTANTS_H__
#define __VS_ERROR_CONSTANTS_H__

//*** Errors ****

#define Vs_Err_lUnableToConvertToVsSimulator -4000
#define Vs_Err_strUnableToConvertToVsSimulator "Unable to convert CAlSimulator to CVsSimulator."

#define Vs_Err_lUnableToConvertToVsRigidBody -4001
#define Vs_Err_strUnableToConvertToVsRigidBody "Unable to convert CAlRigidBody to CVsRigidBody."

#define Vs_Err_lUnableToConvertToVsJoint -4002
#define Vs_Err_strUnableToConvertToVsJoint "Unable to convert CAlJoint to CVsJoint."

#define Vs_Err_lJointToParentNotHingeType -4003
#define Vs_Err_strJointToParentNotHingeType "The joint to the parent is not a hinge type."

#define Vs_Err_lJointConstraintNotDefined -4004
#define Vs_Err_strJointConstraintNotDefined "The joint constraint is not defined."

#define Vs_Err_lLimitIDNotDefined -4006
#define Vs_Err_strLimitIDNotDefined "The limit ID for the joint is not defined."

#define Vs_Err_lUnableToCreateRecorder -4007
#define Vs_Err_strUnableToCreateRecorder "Unable to create the simulation recorder."

#define Vs_Err_lAlreadyHaveActiveRecorder -4008
#define Vs_Err_strAlreadyHaveActiveRecorder "The system is attempting to set an active recorder while another recorder is already active."

#define Vs_Err_lVideoPlaybackNotSupported -4009
#define Vs_Err_strVideoPlaybackNotSupported "Snapshot keyframes do not support video playback capabilities."

#define Vs_Err_lMoveToKeyFrameNotSupported -4010
#define Vs_Err_strMoveToKeyFrameNotSupported "Video keyframes do not support move to key frame capabilities."

#define Vs_Err_lRecorderNotDefined -4011
#define Vs_Err_strRecorderNotDefined "Video recorder is not defined."

#define Vs_Err_lBodyIDNotDefinded -4012
#define Vs_Err_strBodyIDNotDefinded "BodyID is not defined."

#define Vs_Err_lCreatingGeometry -4013
#define Vs_Err_strCreatingGeometry "An error occurred while creating the collision geometry for the speicifed body."

#define Vs_Err_lOpenFile -4014
#define Vs_Err_strOpenFile "Unable to open the specified file."

#define Vs_Err_lInvalidMuscleLengthCols -4015
#define Vs_Err_strInvalidMuscleLengthCols "There can only be 3 items in a muscle length data file: Time, Length, and Velocity"

#define Vs_Err_lMuscleLengthDataEmpty -4016
#define Vs_Err_strMuscleLengthDataEmpty "There was no data in the muscle length file?"

#define Vs_Err_lMuscleLengthTimeStep -4017
#define Vs_Err_strMuscleLengthTimeStep "The time step for the predicted muscle length data does not match the time step of the physics engine."

#define Vs_Err_lMuscleLengthStartTime -4019
#define Vs_Err_strMuscleLengthStartTime "The start and end times for the stimulus and the data file do not match."


#endif // __VS_ERROR_CONSTANTS_H__

