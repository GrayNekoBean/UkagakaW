using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

using UkagakaW.Util;

namespace UkagakaW
{
    public class UkagakaW
    {
        public static Thread LogicThread;

        [DllImport("KiwiRenderer.dll", EntryPoint = "InitializeMainRenderThread", CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitializeMainRenderThread();

        [DllImport("KiwiRenderer.dll", EntryPoint = "InitializeLogicThread", CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitializeLogicThread();

        public static void Main(string[] args)
        {
            Console.WriteLine("wtf");
            IntPtr hDLL = Native.LoadLibrary("KiwiRenderer.dll");

            if (hDLL == IntPtr.Zero)
            {
                string error = Marshal.GetLastWin32Error().ToString();
                string detail = Marshal.GetHRForLastWin32Error().ToString();
                Console.WriteLine(error);
            }
            else
            {
                LogicThread = new Thread(InitializeLogicThread);
                LogicThread.Start();

                Console.WriteLine(hDLL.ToString());
                InitializeMainRenderThread();
            }

            //System.Console.WriteLine("loaded");
            LogicThread.Abort();
            System.Console.ReadLine();
        }
    }
}
