#region Statement

/*
* ---------------------Statement---------------------
* Copyright(c) 2018-2018 Bio-Electric Studio
* All Right Reserved
* This C# source file is for the Application of Project Ukagaka_W.
* You are not allowed to copy any code from here without permission.
*
* File: Ukagaka
* Author: 47339
* Create On: 2018/6/29 16:57:02
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
using System.Runtime.InteropServices;
using UkagakaW.Render;

namespace UkagakaW.Core
{
    public class Ukagaka
    {
        [DllImport("KiwiRenderer.dll", EntryPoint = "SpeakSentence", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SpeakSentence(string ukagakaId, IntPtr words);

        [DllImport("KiwiRenderer.dll", EntryPoint = "NewPhase", CallingConvention = CallingConvention.Cdecl)]
        public static extern void NewPhase(string ukagakaId, IntPtr words);

        public string UkagakaID;

        public UkagakaRenderer renderer;

        public IntPtr hWND;

        public Ukagaka(string ID)
        {
            this.UkagakaID = ID;
            renderer = new UkagakaRenderer(ID);
        }

        public void Say(string words)
        {
            SpeakSentence(this.UkagakaID, Marshal.StringToBSTR(words));
        }

    }
}
