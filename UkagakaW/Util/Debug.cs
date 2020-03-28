#region Statement

/*
* ---------------------Statement---------------------
* Copyright(c) 2018-2020 Bio-Electric Studio
* All Right Reserved
* This C# source file is for the Application of Project Ukagaka_W.
* You are not allowed to copy any code from here without permission.
*
* File: Debug
* Author: Biobean
* Create On: 2020/2/21 14:40:05
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
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace UkagakaW.Util
{
    public static class Debug
    {
        public static bool IsKiwiRendererDebugInfo = false;

        [DllImport("KiwiRenderer.dll", EntryPoint = "DebugLog", CallingConvention = CallingConvention.Cdecl)]
        private extern static void CppLog(IntPtr str);

        [DllImport("KiwiRenderer.dll", EntryPoint = "DebugWarning", CallingConvention = CallingConvention.Cdecl)]
        private extern static void CppWarning(IntPtr str);

        [DllImport("KiwiRenderer.dll", EntryPoint = "DebugError", CallingConvention = CallingConvention.Cdecl)]
        private extern static void CppError(IntPtr str);

        public static void Log(string text)
        {
            if (IsKiwiRendererDebugInfo)
            {
                CppLog(Marshal.StringToBSTR(text));
            }
            else
            {
                System.Console.WriteLine("{0} [Log] UkagakaW: “{1}”", DateTime.Now, text);
            }
        }

        public static void Warning(string text)
        {
            if (IsKiwiRendererDebugInfo)
            {
                CppWarning(Marshal.StringToBSTR(text));
            }
            else
            {
                System.Console.WriteLine("{0} [Warning] UkagakaW: “{1}”", DateTime.Now, text);
            }
        }

        public static void Error(string text)
        {
            if (IsKiwiRendererDebugInfo)
            {
                CppError(Marshal.StringToBSTR(text));
            }
            else
            {
                System.Console.WriteLine("{0} [Error] UkagakaW: “{1}”", DateTime.Now, text);
            }
        }

    }
}
