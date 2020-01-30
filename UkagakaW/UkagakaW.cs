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
            Ukagaka ukagaka = ukagakaInstances[ukagakaID];
            ukagaka.renderer.PlayAnimImmediately("Test-Chan-0", Render.AnimationState.InfinityLoop);

            ukagaka.Say("Hello World, Does it get over to the second line? I mean this string, I guess so." +
                "\n并且，在接下来我想测试一段中文输入。\nちょとまて、日本語はいいんですが？");
            ukagaka.Wait(50);
            //ukagaka.SayInNewPhase("");
            ukagaka.SayInNewPhase("Oh, Oh, don't worry, it's just a test of new phase func.");
        }
    }
}
