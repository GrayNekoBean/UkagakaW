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
    class ScriptRuntime
    {

        ScriptEngine engine;
        ScriptScope scope;

        ScriptSource source;
        CompiledCode compiled;

        public void Initialize()
        {
            this.engine = Python.CreateEngine();
            this.scope = engine.CreateScope();

            source = engine.CreateScriptSourceFromFile("Ukagaka\\Test-Chan\\ghost\\_COMPILED\\_COMPILED.py", Encoding.UTF8, SourceCodeKind.File);
            compiled = source.Compile();

            compiled.Execute(scope);

            scope.SetVariable("ASSEMBLY_UkagakaW", Assembly.GetExecutingAssembly());
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

        public object CallFunction(string func, params object[] pars)
        {
            return null;
        }

    }
}
