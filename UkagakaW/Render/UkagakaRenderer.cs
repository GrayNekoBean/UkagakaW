#region Statement

/*
* ---------------------Statement---------------------
* Copyright(c) 2018-2019 Bio-Electric Studio
* All Right Reserved
* This C# source file is for the Application of Project Ukagaka_W.
* You are not allowed to copy any code from here without permission.
*
* File: UkagakaRenderer
* Author: Biobean
* Create On: 2019/11/18 22:07:01
*
* Overall Description:
* None
*
* Update Description:
* None
*
*
* ---------------------------------------------------
*/

#endregion

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;


namespace UkagakaW.Render
{
    public enum AnimationState
    {
        InfinityLoop = 0,
        EndWithLastFrame = 1
    }

    public class UkagakaRenderer
    {
        [DllImport("KiwiRenderer.dll", EntryPoint = "PlayUkagakaAnimation", CallingConvention = CallingConvention.Cdecl)]
        public static extern void PlayUkagakaAnimation(string ukagakaID, string animID, int animState = 0, float x = 0, float y = 0, float w = 0, float h = 0, float opaque = 1.0f);

        [DllImport("KiwiRenderer.dll", EntryPoint = "PlayUkagakaAnimationImmediately", CallingConvention = CallingConvention.Cdecl)]
        public static extern void PlayUkagakaAnimationImmediately(string ukagakaID, string animID, int animState = 0, float x = 0, float y = 0, float w = 0, float h = 0, float opaque = 1.0f);

        string ukagakaID;

        public UkagakaRenderer(string uid)
        {
            ukagakaID = uid;
        }

        public void PlayAnim(string anim, AnimationState state)
        {
            PlayUkagakaAnimation(this.ukagakaID, anim, (int)state);
        }

        public void PlayAnimImmediately(string anim, AnimationState state)
        {
            PlayUkagakaAnimationImmediately(this.ukagakaID, anim, (int)state);
        }
    }
}
