using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

using UkagakaW.Core;
using UkagakaW.Shiori;
using UkagakaW.Util;

namespace UkagakaW
{
    class UkagakaWMain
    {
        public static SortedDictionary<string, UkagakaInstance> ukagakaInstances;

        public static Thread LogicThread;

        [DllImport("KiwiRenderer.dll", EntryPoint = "InitializeMainRenderThread", CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitializeMainRenderThread();

        [DllImport("KiwiRenderer.dll", EntryPoint = "InitializeLogicThread", CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitializeLogicThread();

        [DllImport("KiwiRenderer.dll", EntryPoint = "PassUkagakaInteractEvent", CallingConvention = CallingConvention.Cdecl)]
        public static extern void PassUkagakaInteractEvent(IntPtr _event);

        [DllImport("KiwiRenderer.dll", EntryPoint = "PassPostInitializeEvent", CallingConvention = CallingConvention.Cdecl)]
        public static extern void PassPostInitializeEvent(IntPtr _event);

        public delegate void UkagakaGeneralEvent(string ukagaka);

        public delegate void UkagakaInteractEvent(string ukagakaID, int par0);

        public delegate void InitFunc();

        static UkagakaInteractEvent interactEvent = UkagakaInteract;
        static UkagakaGeneralEvent postInitialize = UkagakaPostInitialize;
        static IntPtr interactFuncPtr;
        static IntPtr postInitializePtr;

        public static void Main(string[] args)
        {
            Console.WriteLine("wtf");
            ukagakaInstances = new SortedDictionary<string, UkagakaInstance>();

            IntPtr hDLL = Native.LoadLibrary("KiwiRenderer.dll");

            if (hDLL == IntPtr.Zero)
            {
                string error = Marshal.GetLastWin32Error().ToString();
                string detail = Marshal.GetHRForLastWin32Error().ToString();
                Console.WriteLine(error);
            }
            else
            {
                ukagakaInstances.Add("TEST", new UkagakaInstance("TEST"));

                NekoCompiler compiler = new NekoCompiler("Ukagaka\\Test-Chan\\ghost\\NekoScript_Test");
                compiler.Compile();

                interactFuncPtr = Marshal.GetFunctionPointerForDelegate<UkagakaInteractEvent>(interactEvent);
                postInitializePtr = Marshal.GetFunctionPointerForDelegate<UkagakaGeneralEvent>(postInitialize);
                PassUkagakaInteractEvent(interactFuncPtr);
                PassPostInitializeEvent(postInitializePtr);

                LogicThread = new Thread(InitializeLogicThread);
                LogicThread.Start();

                ukagakaInstances["TEST"].ukagaka.AddAnimation("Test-Chan_Smile", new Animation("Test-Chan_Smile"));
                ukagakaInstances["TEST"].ukagaka.AddAnimation("Test-Chan_Surprise", new Animation("Test-Chan_Surprise"));

                Console.WriteLine(hDLL.ToString());
                InitializeMainRenderThread();
            }

            //System.Console.WriteLine("loaded");
            LogicThread.Abort();
            Native.FreeLibrary(hDLL);
            System.Console.ReadLine();
        }

        public static void UkagakaPostInitialize(string ukagakaID)
        {
            ukagakaInstances[ukagakaID].InitializeScript();
        }

        public static void UkagakaInteract(string ukagakaID, int parameter)
        {
            Console.WriteLine(ukagakaID + " Interacted!");
            UkagakaInstance instance = ukagakaInstances[ukagakaID];
            Ukagaka ukagaka = instance.ukagaka;

            instance.RunEvent(UkagakaEvent.FIRST_COMEOUT);

            ukagaka.renderer.PlayAnimImmediately("Test-Chan_Smile", Render.AnimationState.InfinityLoop);
            ukagaka.Say("Hello World, Does it get over to the second line? I mean this string, I guess so." +
                "\n并且，在接下来我想测试一段中文输入。\nちょとまて、日本語はいいんですが？");
            ukagaka.Wait(50);
            ukagaka.NewPhase();
            ukagaka.Say("Oh, Oh, don't worry, it's just a test of new phase func.");
            ukagaka.Finish();
        }
    }
}
