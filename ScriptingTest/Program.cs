using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Microsoft.Scripting;
using Microsoft.Scripting.Hosting;
using IronPython.Hosting;
using System.Windows;
using System.Reflection;

namespace ScriptingTest
{
    public class Program
    {

        static void Main(string[] args)
        {
            ScriptRuntime runtime = new ScriptRuntime();
            Console.WriteLine("finished");
            Console.ReadLine();
        }

        public static void Test1(string arg)
        {
            MessageBox.Show("Test1:" + arg);
        }
    }

    public class TestCls
    {
        public string parameter;

        public TestCls(string par)
        {
            this.parameter = par;
        }

        public void Test2()
        {
            MessageBox.Show("Test2:" + parameter);
        }
    }

    public class ScriptRuntime
    {

        ScriptEngine engine;
        ScriptScope scope;

        ScriptSource source;
        CompiledCode compiled;

        public ScriptRuntime()
        {
            this.engine = Python.CreateEngine();
            this.scope = engine.CreateScope();

            this.source = engine.CreateScriptSourceFromString(
@"

scriptingTest = 'ScriptingTest'

import clr
clr.AddReference(scriptingTest)

import ScriptingTest

cls = ScriptingTest.TestCls('hhh')
cls.Test2()
", SourceCodeKind.Statements);
            this.compiled = source.Compile();

            //scope.SetVariable("scriptingTest", Assembly.GetExecutingAssembly());
            //scope.SetVariable("cls", new TestCls("hahaha"));

            source.Execute();
        }
    }
}
