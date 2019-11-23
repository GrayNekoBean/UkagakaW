using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

using UkagakaW.Core;
using UkagakaW.Util;

namespace UkagakaW
{
    public class UkagakaW
    {
        public static Dictionary<string, Ukagaka> ukagakaInstances = new Dictionary<string, Ukagaka>();

        public static Thread LogicThread;

        [DllImport("KiwiRenderer.dll", EntryPoint = "InitializeMainRenderThread", CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitializeMainRenderThread();

        [DllImport("KiwiRenderer.dll", EntryPoint = "InitializeLogicThread", CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitializeLogicThread();

        [DllImport("KiwiRenderer.dll", EntryPoint = "PassUkagakaInteractEvent", CallingConvention = CallingConvention.Cdecl)]
        public static extern void PassUkagakaInteractEvent(IntPtr _event);

        public delegate void UkagakaInteractEvent(string ukagakaID, int par0);

        public delegate void InitFunc();

        static UkagakaInteractEvent interactEvent = UkagakaInteract;
        static IntPtr interactFuncPtr;

        public static void Main(string[] args)
        {
            Console.WriteLine("wtf");
            ukagakaInstances = new Dictionary<string, Ukagaka>();

            IntPtr hDLL = Native.LoadLibrary("KiwiRenderer.dll");

            if (hDLL == IntPtr.Zero)
            {
                string error = Marshal.GetLastWin32Error().ToString();
                string detail = Marshal.GetHRForLastWin32Error().ToString();
                Console.WriteLine(error);
            }
            else
            {
                ukagakaInstances.Add("TEST", new Ukagaka("TEST"));

                interactFuncPtr = Marshal.GetFunctionPointerForDelegate<UkagakaInteractEvent>(interactEvent);
                PassUkagakaInteractEvent(interactFuncPtr);

                LogicThread = new Thread(InitializeLogicThread);
                LogicThread.Start();

                Console.WriteLine(hDLL.ToString());
                InitializeMainRenderThread();
            }

            //System.Console.WriteLine("loaded");
            LogicThread.Abort();
            Native.FreeLibrary(hDLL);
            System.Console.ReadLine();
        }

        public static void UkagakaInteract(string ukagakaID, int parameter)
        {
            Console.WriteLine(ukagakaID + " Interacted!");
            ukagakaInstances[ukagakaID].renderer.PlayAnimImmediately("Test-Chan-0", Render.AnimationState.InfinityLoop);

            ukagakaInstances[ukagakaID].Say("helloWorld");
        }
    }
}
