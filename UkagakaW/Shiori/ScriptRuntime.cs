#region Statement

/*
* ---------------------Statement---------------------
* Copyright(c) 2018-2019 Bio-Electric Studio
* All Right Reserved
* This C# source file is for the Application of Project Ukagaka_W.
* You are not allowed to copy any code from here without permission.
*
* File: ScriptRuntime
* Author: Biobean
* Create On: 2019/12/5 20:11:13
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
using System.Text;
using System.Threading.Tasks;
using System.Reflection;

using UkagakaW.Core;

using Microsoft.Scripting;
using IronPython;
using Microsoft.Scripting.Hosting;
using IronPython.Hosting;

namespace UkagakaW.Shiori
{

    public class ScriptRuntime
    {

        protected ScriptEngine engine;
        protected ScriptScope scope;

        protected ScriptSource source;
        protected CompiledCode compiled;

        public ScriptRuntime(string path)
        {
            Util.Debug.Log("Compiling Python Scripts with IronPython 2.7...");
            this.engine = Python.CreateEngine();
            this.scope = engine.CreateScope();

            source = engine.CreateScriptSourceFromFile(path, Encoding.UTF8, SourceCodeKind.File);
            compiled = source.Compile();
            Util.Debug.Log("Python Scripts Compiled done.");

            scope.SetVariable("ASSEMBLY_UkagakaW", Assembly.GetExecutingAssembly());
        }

        public void Execute()
        {
            Util.Debug.Log("Starting Setup Script...");
            compiled.Execute(scope);
            Util.Debug.Log("Done.");
        }

        public dynamic CallFunction(dynamic func, params object[] parameters)
        {
            return engine.Operations.Invoke(func, parameters);
        }

        public dynamic CallFunctionByName(string funcName, params object[] parameters)
        {
            if (parameters == null || parameters.Length == 0)
            {
                return engine.Operations.Invoke(scope.GetVariable(funcName));
            }
            else
            {
                return engine.Operations.Invoke(scope.GetVariable(funcName), parameters);
            }
        }

        public dynamic CallMemberFunctionByName(object obj, string memberFunc, params object[] parameters)
        {
            return engine.Operations.InvokeMember(obj, memberFunc, parameters);
        }
    }

    public class UkagakaScriptRuntime : ScriptRuntime
    {

        Dictionary<UkagakaEvent, dynamic> EventTable = new Dictionary<UkagakaEvent, dynamic>();

        public UkagakaScriptRuntime(string path) : base(path)
        {
            EventTable = new Dictionary<UkagakaEvent, dynamic>();
        }

        public void Initialize()
        {
            compiled.Execute(scope);
        }

        public void UpdateScriptRuntime()
        {

        }

        public bool SetUkagaka(string ukagakaVar, Ukagaka value)
        {
            scope.SetVariable(ukagakaVar, value);
            return true;
        }

        public void SetAnimation(string animationVar, string anim)
        {
            //scope.SetVariable(animationVar, new Animation)
        }

        public void RegisterGlobalEvent(GlobalEvent eventID, dynamic EventFunc)
        {

        }

        public void RegisterUkagakaEvent(UkagakaEvent type,  dynamic EventFunc)
        {
            if (!EventTable.ContainsKey(type))
            {
                EventTable.Add(type, EventFunc);
            }
            else
            {
                EventTable[type] = EventFunc;
            }
        }

        public void RunUkagakaEvent(UkagakaEvent event_, params dynamic[] pars)
        {
            CallFunction(EventTable[event_], pars);
        }
    }
}
