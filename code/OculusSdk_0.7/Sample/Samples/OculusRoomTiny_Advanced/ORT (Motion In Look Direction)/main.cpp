﻿/************************************************************************************
Filename    :   Win32_RoomTiny_Main.cpp
Content     :   First-person view test application for Oculus Rift
Created     :   18th Dec 2014
Authors     :   Tom Heath
Copyright   :   Copyright 2012 Oculus, Inc. All Rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*************************************************************************************/
/// A simple demo to show the extra code needed to have the player's movement
/// (from WASD and cursors) in the direction of where the player is looking (including Rift
/// orientations), not just in the direction of the player (independent of Rift orientation)
/// as exhibited in most of these samples.

#define   OVR_D3D_VERSION 11
#include "..\Common\Old\Win32_DirectXAppUtil.h" // DirectX
#include "..\Common\Old\Win32_BasicVR.h"  // Basic VR

//-------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hinst, HINSTANCE, LPSTR, int)
{
    BasicVR basicVR(hinst);
    basicVR.Layer[0] = new VRLayer(basicVR.HMD);

    // Main loop
    while (basicVR.HandleMessages())
    {
        // We pass in zero, to make no positional movement
        // but keep rest of the motion intact.
        basicVR.ActionFromInput(0);

        basicVR.Layer[0]->GetEyePoses();

        // Find the orthogonal vectors resulting from combined rift and user yaw
        XMVECTOR totalRot = XMQuaternionMultiply(ConvertToXM(basicVR.Layer[0]->EyeRenderPose[0].Orientation), basicVR.MainCam.Rot);
        XMVECTOR forward = XMVector3Rotate(XMVectorSet(0, 0, -1, 0), totalRot);
        XMVECTOR right   = XMVector3Rotate(XMVectorSet(1, 0, 0, 0), totalRot);

        // Keyboard inputs to adjust player position, using these orthogonal vectors
        float speed = 0.05f;
        if (DIRECTX.Key['W'] || DIRECTX.Key[VK_UP])   basicVR.MainCam.Pos = XMVectorAdd(basicVR.MainCam.Pos, XMVectorScale(forward,+speed));
        if (DIRECTX.Key['S'] || DIRECTX.Key[VK_DOWN]) basicVR.MainCam.Pos = XMVectorAdd(basicVR.MainCam.Pos, XMVectorScale(forward,-speed));
        if (DIRECTX.Key['D'])                         basicVR.MainCam.Pos = XMVectorAdd(basicVR.MainCam.Pos, XMVectorScale(right, +speed));
        if (DIRECTX.Key['A'])                         basicVR.MainCam.Pos = XMVectorAdd(basicVR.MainCam.Pos, XMVectorScale(right, -speed));

        for (int eye = 0; eye < 2; eye++)
        {
            basicVR.Layer[0]->RenderSceneToEyeBuffer(&basicVR.MainCam, &basicVR.RoomScene, eye);
        }

        basicVR.Layer[0]->PrepareLayerHeader();
        basicVR.DistortAndPresent(1);
    }

    return (basicVR.Release(hinst));
}
