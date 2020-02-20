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
using System.Runtime.InteropServices;
using UkagakaW.Render;

namespace UkagakaW.Core
{

    public enum UkagakaEvent
    {
        FIRST_COMEOUT = 0,
        TOUCH_HEAD = 1
    }

    public enum GlobalEvent
    {
        Initialize = 0,
        EndProgram = 1
    }

    public class UkagakaInstance
    {
        public Ukagaka ukagaka;

        private Shiori.UkagakaScriptRuntime scriptRuntime;

        public UkagakaInstance(string ID)
        {
            ukagaka = new Ukagaka(ID);
            scriptRuntime = new Shiori.UkagakaScriptRuntime("Ukagaka\\Test-Chan\\ghost\\COMPILED\\_COMPILED.py");
        }

        public void RunEvent(UkagakaEvent event_, params dynamic[] pars)
        {
            scriptRuntime.RunUkagakaEvent(event_, pars);
        }

        public void InitializeScript()
        {
            scriptRuntime.Initialize();
        }

        public void RegEvent(UkagakaEvent EventID, dynamic EventFunc)
        {
            scriptRuntime.RegisterUkagakaEvent(EventID, EventFunc);
        }
    }

    public class Ukagaka
    {
        [DllImport("KiwiRenderer.dll", EntryPoint = "OutputText", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SpeakSentence(string ukagakaId, IntPtr words, int style, int color);

        [DllImport("KiwiRenderer.dll", EntryPoint = "SetNewPhase", CallingConvention = CallingConvention.Cdecl)]
        private static extern void StartNewPhase(string ukagakaId);

        [DllImport("KiwiRenderer.dll", EntryPoint = "WaitForTicks", CallingConvention = CallingConvention.Cdecl)]
        private static extern void WaitForTick(string ukagakaId, int ticks);

        [DllImport("KiwiRenderer.dll", EntryPoint = "EndSection", CallingConvention = CallingConvention.Cdecl)]
        private static extern void EndSpeak(string ukagakaId);

        [DllImport("KiwiRenderer.dll", EntryPoint = "CreateUserDefinedFont", CallingConvention = CallingConvention.Cdecl)]
        private static extern int CreateUserDefinedFont(string ukagakaID, IntPtr font, float size, bool bold, bool italic);

        [DllImport("KiwiRenderer.dll", EntryPoint = "CreateUserDefinedColor", CallingConvention = CallingConvention.Cdecl)]
        private static extern int CreateUserDefinedColor(string ukagakaID, float r, float g, float b, float a);

        public string UkagakaID;

        private System.Collections.Generic.Dictionary<string, Animation> AnimationTable;

        public UkagakaRenderer renderer;

        public IntPtr hWND;

        public Ukagaka(string ID)
        {
            this.UkagakaID = ID;
            renderer = new UkagakaRenderer(ID);
            AnimationTable = new System.Collections.Generic.Dictionary<string, Animation>();
        }

        public void Say(string words, int font = 0, int color = 0)
        {
            SpeakSentence(this.UkagakaID, Marshal.StringToBSTR(words), font, color);
        }

        public void NewPhase()
        {
            StartNewPhase(this.UkagakaID);
        }

        public void Express(string anim)
        {
            renderer.PlayAnimImmediately(anim, AnimationState.InfinityLoop);
        }

        public void Express(Animation anim)
        {
            renderer.PlayAnimImmediately(anim.animationID, anim.defaultState);
        }

        public void Wait(int ticks)
        {
            WaitForTick(this.UkagakaID, ticks);
        }

        public void WaitForTicks(int ticks)
        {
            WaitForTick(this.UkagakaID, ticks);
        }

        public void WaitForMilliseconds(int ms)
        {
            //TODO: Wait for milliseconds API code Implementation
        }

        public void Finish()
        {
            EndSpeak(this.UkagakaID);
        }

        public Animation GetAnimation(string animId)
        {
            return AnimationTable[animId];
        }

        public void AddAnimation(string ID, Animation animation)
        {
            AnimationTable.Add(ID, animation);
        }

        public int CreateFont(string font, int size, bool bold, bool italic)
        {
            return CreateUserDefinedFont(this.UkagakaID, Marshal.StringToBSTR(font), size, bold, italic);
        }

        public int MakeColor(int r, int g, int b, int a = 255)
        {
            return CreateUserDefinedColor(this.UkagakaID, (float)r, (float)g, (float)b, (float)a);
        }

        public int MakeColor(int[] color)
        {
            if(color.Length == 4)
            {
                return CreateUserDefinedColor(this.UkagakaID, (float)color[0], (float)color[1], (float)color[2], (float)color[3]);
            }
            else if(color.Length == 3)
            {
                return CreateUserDefinedColor(this.UkagakaID, (float)color[0], (float)color[1], (float)color[2], 255.0f);
            }
            else
            {
                return -1;
            }
        }
           
        public void RegisterUkagakaEvent(UkagakaEvent EventID, dynamic EventFunc)
        {
            UkagakaWMain.ukagakaInstances[this.UkagakaID].RegEvent(EventID, EventFunc);
        }
    }
}
