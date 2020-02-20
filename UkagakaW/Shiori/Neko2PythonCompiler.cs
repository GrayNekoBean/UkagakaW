#region Statement

/*
* ---------------------Statement---------------------
* Copyright(c) 2018-2019 Bio-Electric Studio
* All Right Reserved
* This C# source file is for the Application of Project Ukagaka_W.
* You are not allowed to copy any code from here without permission.
*
* File: Neko2PythonCompiler
* Author: Biobean
* Create On: 2019/11/18 21:57:48
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

//Some of the code referenced from https://stackoverflow.com/questions/673113/poor-mans-lexer-for-c-sharp/673657#673657

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

using UkagakaW.Core;

namespace UkagakaW.Shiori
{

    public struct ActionFunc
    {
        public string action;
        public string[] parameters;

        public string function;

        public string GetParameterListString()
        {
            StringBuilder builder = new StringBuilder();

            foreach(object p in parameters)
            {
                builder.Append(p.ToString());
                builder.Append(",");
            }

            builder.Remove(builder.Length - 1, 1);

            return builder.ToString();
        }

        public static string GetParameterListString(string[] pars)
        {
            StringBuilder builder = new StringBuilder();

            foreach (string p in pars)
            {
                builder.Append(p);
                builder.Append(",");
            }

            builder.Remove(builder.Length - 1, 1);

            return builder.ToString();
        }
    }

    enum TokenType
    {
        field,
        text,
        str,
        str_single,
        str_double,

        key_define,  //define
        key_anim,  //anim
        key_ukagaka,  //ukagaka
        key_event,  //event
        key_action,  //action
        key_of,  //of
        key_function,  //function

        symbol_at,  // @
        symbol_colon,  // :
        symbol_end,  // |
        symbol_bar,  // -
        symbol_dollar,  //$

        symbol_angle_left,  // <
        symbol_angle_right,  // >
        symbol_bracket_left,
        symbol_bracket_right,
        symbol_quote_double,  //"
        symbol_quote,  // '
        symbol_comma,  //,
        symbol_dot,  //.

        EOL,  //End Of Line
        EOF,  //End Of File
        PythonLine,
        PythonLine_Variable,
        Empty,
        Unrecognize
    }

    enum NekoCompilerErrorType
    {
        TwiceDefine,
        ParameterError,
        UndefinedObject
    }

    class NekoCompilerError
    {
        NekoCompilerErrorType type;
        string error;
        int line;
    }

    class NekoToken
    {
        public int line;

        public TokenType tokenType;
        public string Value;
    }

    class TokenDefinitionCollection
    {
        private List<TokenDefinition> tokenDefinitions;

        public TokenDefinitionCollection()
        {
            tokenDefinitions = new List<TokenDefinition>();
        }

        public TokenDefinition[] ToArray()
        {
            return tokenDefinitions.ToArray();
        }

        public void AddTokenDef(IMatcher matcher, TokenType token)
        {
            this.tokenDefinitions.Add(new TokenDefinition(matcher, token));
        }

        public void AddSymbolTokenDef(char symbol, TokenType token)
        {
            this.tokenDefinitions.Add(new TokenDefinition(symbol, token));
        }

        public void AddKeywordTokenDef(string keyword, TokenType token)
        {
            this.tokenDefinitions.Add(new TokenDefinition(keyword, token));
        }

        public void AddRegexTokenDef(string regex, TokenType token)
        {
            this.tokenDefinitions.Add(new TokenDefinition(new RegexMatcher(
                new Regex(String.Format("^\\s*{0}", regex))),
                token
                ));
        }

    }

    public class NekoCompiler
    {

        class UkagakaPrivateScriptData
        {

            public Dictionary<string, string> definedName2variableNameTable_private;
            public Dictionary<string, ActionFunc> privateActions;
            public Dictionary<string, dynamic> privateEvents;

            public UkagakaPrivateScriptData()
            {
                definedName2variableNameTable_private = new Dictionary<string, string>();
                privateActions = new Dictionary<string, ActionFunc>();
                privateEvents = new Dictionary<string, dynamic>();
            }
        }

        public static NekoCompiler Current;

        private ScriptRuntime tempRuntime;

        private StringBuilder compiledPythonScript;

        private string sourcePath;

        private TextWriter scriptWriter;

        private Dictionary<string, ActionFunc> globalActions;

        private Dictionary<string, dynamic> globalEvents;

        private Dictionary<string, UkagakaPrivateScriptData> ukagakaDatas; //<UkagakaID, Data>

        private Dictionary<string, string> definedName2variableNameTable_global;

        private List<NekoToken> tokenList;

        private Dictionary<int, int> lineOfIndentLevel;

        private TokenDefinition[] ScriptDefinitions;

        private TokenDefinition[] NekoExpressionDefinitions;

        private List<string> preCompiles;

        private List<string> tailLines;

        Lexer lexer;

        public NekoCompiler(string directoryPath)
        {
            this.sourcePath = directoryPath;

            this.compiledPythonScript = new StringBuilder();

            globalActions = new Dictionary<string, ActionFunc>();
            globalEvents = new Dictionary<string, dynamic>();
            ukagakaDatas = new Dictionary<string, UkagakaPrivateScriptData>();
            definedName2variableNameTable_global = new Dictionary<string, string>();

            tokenList = new List<NekoToken>();
            lineOfIndentLevel = new Dictionary<int, int>();

            preCompiles = new List<string>();
            tailLines = new List<string>();

            Current = this;
        }

        private void AddTokenDefinitions()
        {
            TokenDefinitionCollection scriptDefs = new TokenDefinitionCollection();

            TokenDefinitionCollection nekoExpDefs = new TokenDefinitionCollection();

            scriptDefs.AddRegexTokenDef("(\'.*\')", TokenType.str);
            scriptDefs.AddRegexTokenDef("(\".*\")", TokenType.str);

            scriptDefs.AddSymbolTokenDef('#', TokenType.PythonLine);

            scriptDefs.AddKeywordTokenDef("from", TokenType.PythonLine);
            scriptDefs.AddKeywordTokenDef("import", TokenType.PythonLine);
            scriptDefs.AddKeywordTokenDef("def", TokenType.PythonLine);
            scriptDefs.AddRegexTokenDef("\\$(.+)\b=\b.+", TokenType.PythonLine_Variable);
            scriptDefs.AddRegexTokenDef(@".+\(.*\)", TokenType.PythonLine);
            scriptDefs.AddSymbolTokenDef('=', TokenType.PythonLine);
            scriptDefs.AddKeywordTokenDef("print", TokenType.PythonLine);
            scriptDefs.AddKeywordTokenDef("if", TokenType.PythonLine);
            scriptDefs.AddKeywordTokenDef("else", TokenType.PythonLine);
            scriptDefs.AddKeywordTokenDef("elif", TokenType.PythonLine);
            scriptDefs.AddKeywordTokenDef("for", TokenType.PythonLine);
            scriptDefs.AddKeywordTokenDef("del", TokenType.PythonLine);
            scriptDefs.AddKeywordTokenDef("try", TokenType.PythonLine);
            scriptDefs.AddKeywordTokenDef("except", TokenType.PythonLine);
            scriptDefs.AddKeywordTokenDef("finally", TokenType.PythonLine);
            scriptDefs.AddKeywordTokenDef("while", TokenType.PythonLine);
            scriptDefs.AddKeywordTokenDef("return", TokenType.PythonLine);
            scriptDefs.AddKeywordTokenDef("class", TokenType.PythonLine);

            scriptDefs.AddKeywordTokenDef("define", TokenType.key_define);
            scriptDefs.AddKeywordTokenDef("ukagaka", TokenType.key_ukagaka);
            scriptDefs.AddKeywordTokenDef("anim", TokenType.key_anim);
            scriptDefs.AddKeywordTokenDef("action", TokenType.key_action);
            scriptDefs.AddKeywordTokenDef("event", TokenType.key_event);
            scriptDefs.AddKeywordTokenDef("of", TokenType.key_of);

            scriptDefs.AddKeywordTokenDef("function", TokenType.key_function);

            scriptDefs.AddSymbolTokenDef(':', TokenType.symbol_colon);

            scriptDefs.AddSymbolTokenDef(',', TokenType.symbol_comma);
            scriptDefs.AddSymbolTokenDef('(', TokenType.symbol_bracket_left);
            scriptDefs.AddSymbolTokenDef(')', TokenType.symbol_bracket_right);

            scriptDefs.AddTokenDef(new GeneralFieldMatcher(), TokenType.field);

            scriptDefs.AddRegexTokenDef(".*", TokenType.Unrecognize);

            nekoExpDefs.AddSymbolTokenDef('|', TokenType.symbol_end);

            nekoExpDefs.AddTokenDef(new RegexMatcher(new Regex("^(\\s*)$")), TokenType.Empty);
            nekoExpDefs.AddTokenDef(new GeneralFieldMatcher(true), TokenType.field);
            nekoExpDefs.AddTokenDef(new TextMatcher(new char[] { '@', '$', '<', '>', '|', '"'}), TokenType.text);
            nekoExpDefs.AddSymbolTokenDef('@', TokenType.symbol_at);
            nekoExpDefs.AddSymbolTokenDef('$', TokenType.symbol_dollar);
            nekoExpDefs.AddSymbolTokenDef(',', TokenType.symbol_comma);
            nekoExpDefs.AddSymbolTokenDef('(', TokenType.symbol_bracket_left);
            nekoExpDefs.AddSymbolTokenDef(')', TokenType.symbol_bracket_right);
            nekoExpDefs.AddSymbolTokenDef('<', TokenType.symbol_angle_left);
            nekoExpDefs.AddSymbolTokenDef('>', TokenType.symbol_angle_right);
            nekoExpDefs.AddRegexTokenDef("('.*')", TokenType.str_single);
            nekoExpDefs.AddRegexTokenDef("\"(.*)\"", TokenType.str_double);

            nekoExpDefs.AddRegexTokenDef(".*", TokenType.Unrecognize);


            this.ScriptDefinitions = scriptDefs.ToArray();
            this.NekoExpressionDefinitions = nekoExpDefs.ToArray();
        }

        public void Compile()
        {

            this.AddTokenDefinitions();

            Combine();

            //File.CreateText("Ukagaka\\Test-Chan\\ghost\\COMPILED\\_COMPILED.py");
            FileStream stream = File.Open("Ukagaka\\Test-Chan\\ghost\\COMPILED\\_PRECOMPILE.py", FileMode.OpenOrCreate);
            scriptWriter = new StreamWriter(stream);

            TextReader reader = new StreamReader("Ukagaka\\Test-Chan\\ghost\\TEMP\\_COMBINED.neko");

            lexer = new Lexer(reader, ScriptDefinitions);

            PreCompile();

            reader.Close();

            WritePreCompiles();

            scriptWriter.Close();

            //Just for test
            //WriteTails();

            tempRuntime = new ScriptRuntime("Ukagaka\\Test-Chan\\ghost\\COMPILED\\_PRECOMPILE.py");
            tempRuntime.Execute();
            tempRuntime.CallFunctionByName("PRE_COMPILE");

            File.Delete("Ukagaka\\Test-Chan\\ghost\\COMPILED\\_PRECOMPILE.py");

            File.Delete("Ukagaka\\Test-Chan\\ghost\\COMPILED\\_COMPILED.py");
            stream = File.Open("Ukagaka\\Test-Chan\\ghost\\COMPILED\\_COMPILED.py", FileMode.OpenOrCreate);
            scriptWriter = new StreamWriter(stream);

            reader = new StreamReader("Ukagaka\\Test-Chan\\ghost\\TEMP\\_COMBINED.neko");

            lexer = new Lexer(reader, ScriptDefinitions);

            WriteHeadlines();

            WriteScript();

            PostCompile();

            WriteScript();

            WriteTails();

            reader.Close();
        }

        /*
         *  Combine all the neko script in to a "_COMBINED.neko" file
         */
        private void Combine()
        {
            Directory.CreateDirectory("Ukagaka\\Test-Chan\\ghost\\TEMP");
            //File.CreateText("Ukagaka\\Test-Chan\\ghost\\TEMP\\_COMBINED.neko");
            File.Delete("Ukagaka\\Test-Chan\\ghost\\TEMP\\_COMBINED.neko");
            FileStream stream = File.Open("Ukagaka\\Test-Chan\\ghost\\TEMP\\_COMBINED.neko", FileMode.OpenOrCreate);
            StreamWriter writer = new StreamWriter(stream);
            foreach (string p in Directory.GetFiles(sourcePath, "*.neko", SearchOption.AllDirectories))
            {
                TextReader reader = new StreamReader(p);
                string fileName = Path.GetFileName(p);
                writer.WriteLine(String.Format("#--------------Begin write file {0}------------------", fileName));
                string script = reader.ReadToEnd();
                writer.Write(script);
                writer.WriteLine(String.Format("#--------------End of file {0}------------------", fileName));
                reader.Close();
            }
            writer.Close();
        }

        /*
         * Read configuration in comments
         * Compile all the defines to some variable statement in Python, and generate a PRE_COMPILE() function
         * After that, run the PRE_COMPILE function, and ready to begin the next phase of compilation
         */
        private void PreCompile()
        {
            bool defining = false;
            Queue<string> buffer = new Queue<string>();

            bool readPar = false;
            List<string> parameters = new List<string>();

            bool readPreCompile = false;

            TokenType tk = TokenType.field;
            string val = "";
            while (lexer.Next())
            {
                tk = lexer.Token;
                val = lexer.TokenContents;

                if (readPreCompile)
                {
                    if (lexer.TokenContents != "pass")
                    {
                        this.preCompiles.Add(lexer.Line);
                        lexer.NextLine();
                        continue;
                    }
                    else
                    {
                        readPreCompile = false;
                        continue;
                    }
                }

                if (defining)
                {
                    if (tk != TokenType.EOL)
                    {
                        if (!readPar)
                        {
                            if (tk == TokenType.symbol_bracket_left)
                            {
                                readPar = true;
                            }
                            else
                            {
                                if (tk != TokenType.key_of && tk != TokenType.key_define)
                                {
                                    buffer.Enqueue(val);
                                }
                            }
                        }
                        else
                        {
                            if(tk == TokenType.field || tk == TokenType.str)
                            {
                                parameters.Add(val);
                            }else if(lexer.Token == TokenType.symbol_comma)
                            {

                            }else if(tk == TokenType.symbol_bracket_right)
                            {
                                readPar = false;
                            }
                            else
                            {
                                throw new Exception("unexpected symbol in action define");
                            }
                        }
                    }
                    else
                    {
                        defining = false;

                        string defType = buffer.Dequeue();
                        if(defType == "anim")
                        {
                            string animID = buffer.Dequeue();
                            string defName = buffer.Dequeue();

                            string uka = buffer.Dequeue();

                            AppendLine(DefineUkagakaAnimation(defName, animID, uka));

                        }else if(defType == "ukagaka")
                        {
                            string ukagakaID = buffer.Dequeue();
                            string defName = buffer.Dequeue();

                            AppendLine(DefineUkagaka(defName, ukagakaID));
                        }else if(defType == "event")
                        {
                            string eventID = buffer.Dequeue();
                            string eventFunc = buffer.Dequeue();

                            if(buffer.Count > 0)
                            {
                                string ukagaka = buffer.Dequeue();

                                AppendLine(DefineUkagakaEvent(ukagaka, eventID, eventFunc));
                            }
                            else
                            {
                                AppendLine(DefineGlobalEvent(eventID, eventFunc));
                            }
                        }else if(defType == "action")
                        {
                            string actionFunc = buffer.Dequeue();
                            string defName = buffer.Dequeue();

                            if(buffer.Count > 0)
                            {
                                string ukagaka = buffer.Dequeue();

                                AppendLine(DefineUkagakaAction(ukagaka, defName, actionFunc, parameters.ToArray()));
                            }
                            else
                            {
                                AppendLine(DefineGlobalAction(defName, actionFunc, parameters.ToArray()));
                            }
                        }
                        else
                        {
                            throw new Exception("unexpected type of object to define, if you want to state a variable other than ukagaka, animation, action or event, please use standard python code");
                        }
                    }
                    continue;
                }

                bool readingEvent = false;

                if (readingEvent)
                {
                    if(tk == TokenType.field)
                    {
                        readingEvent = false;
                        lexer.NextLine();
                        if (val == "PRE_COMPILE")
                        {
                            readPreCompile = true;
                            continue;
                        }
                    }
                }

                if(lexer.Token == TokenType.key_define)
                {
                    defining = true;
                }else if(lexer.Token == TokenType.key_event)
                {
                    readingEvent = true;

                }
                else
                {
                    lexer.NextLine();
                    continue;
                }
            }
        }

        /*
         * Reunion the indent in the code
         * Compile all the Neko Expression and statement block(like Random: Select:) into Python codes
         *  and finish the compilation
         */
        private void PostCompile()
        {
            NekoToken previousToken = null;
            TokenType previousTkType = TokenType.field;
            bool inNekoExp = false;
            string currentUkagaka = null, currentUkagakaVar = null;

            TokenType[] criticalTokens = new TokenType[]
            {
               TokenType.symbol_at, TokenType.symbol_dollar, TokenType.symbol_angle_left, TokenType.symbol_angle_right, TokenType.symbol_end, TokenType.str_double
            };
            StringBuilder text = new StringBuilder();

            bool isNodeGlobal = false;
            bool nodeFlushIn = false;
            bool hasPar = false;
            Queue<string> nodeBuffer = new Queue<string>();

            while (lexer.Next())
            {


                TokenType tkType = lexer.Token;

                NekoToken token = new NekoToken()
                {
                    line = lexer.LineNumber,
                    tokenType = tkType,
                    Value = lexer.TokenContents
                };
                tokenList.Add(token);

                if (tkType == TokenType.key_define)
                {
                    previousToken = token;
                    previousTkType = tkType;

                    lexer.NextLine();
                    continue;
                }

                if (tkType != TokenType.Unrecognize && tkType != TokenType.PythonLine)
                {

                    if(previousTkType == TokenType.key_event || previousTkType == TokenType.key_function)
                    {
                        if (tkType == TokenType.field)
                        {
                            AppendLine(String.Format("def {0}():", token.Value));

                            previousToken = token;
                            previousTkType = tkType;

                            lexer.NextLine();
                            continue;
                        }
                    }

                    if (inNekoExp)
                    {

                        if (previousTkType == TokenType.symbol_at)
                        {
                            if (globalActions.ContainsKey(lexer.TokenContents))
                            {
                                nodeFlushIn = true;
                                isNodeGlobal = true;
                            }
                            else if (ukagakaDatas[currentUkagaka].privateActions.ContainsKey(token.Value))
                            {
                                nodeFlushIn = true;
                                isNodeGlobal = false;
                            }
                            else
                            {
                                throw new Exception("Undefined node");
                            }
                        }

                        if (nodeFlushIn)
                        {
                            
                            if(tkType == TokenType.symbol_bracket_left)
                            {
                                hasPar = true;
                            }
                            else if (tkType == TokenType.symbol_bracket_right || tkType == TokenType.symbol_at || tkType == TokenType.symbol_angle_left 
                                || tkType == TokenType.str_double || tkType == TokenType.EOL || (!hasPar && (tkType == TokenType.Empty || tkType == TokenType.text)))
                            {
                                nodeFlushIn = false;

                                string actionName = nodeBuffer.Dequeue();
                                string[] actionPars = { };

                                if (nodeBuffer.Count > 0)
                                {
                                    nodeBuffer.ToArray();
                                }

                                if (isNodeGlobal)
                                {
                                    this.AppendLine(
                                        this.ConvertGlobalAction(actionName, actionPars)
                                        );
                                }
                                else
                                {
                                    this.AppendLine(
                                        this.ConvertUkagakaAction(currentUkagaka, actionName, actionPars)
                                    );
                                }

                                nodeFlushIn = false;
                                hasPar = false;
                            }
                            else if (tkType == TokenType.field || tkType == TokenType.str)
                            {
                                nodeBuffer.Enqueue(token.Value);
                            }
                            else
                            {
                                if (tkType != TokenType.Empty || tkType != TokenType.symbol_comma)
                                {
                                    throw new Exception("unexpected token in the action node");
                                }
                            }
                        }
                        
                        if(!nodeFlushIn)
                        {
                            if (!criticalTokens.Contains<TokenType>(tkType))
                            {
                                if (tkType != TokenType.symbol_quote)
                                {
                                    text.Append(token.Value);
                                }
                                else
                                {
                                    text.Append("\\" + token.Value);
                                }

                            }
                            else
                            {
                                string txt = text.ToString();
                                if (txt.Replace(" ", "").Length > 0)
                                {
                                    AppendLine(String.Format("{0}.Say('{1}')", currentUkagakaVar, txt.Trim()));
                                    text.Clear();
                                }
                            }
                        }

                        if (tkType == TokenType.symbol_end)
                        {
                            if (currentUkagakaVar != null && currentUkagakaVar != "")
                            {
                                AppendLine(String.Format("{0}.Finish()", currentUkagakaVar));
                            }

                            nodeFlushIn = false;
                            nodeBuffer.Clear();

                            inNekoExp = false;
                            lexer.SwitchTokenDefinationCollection(ScriptDefinitions);
                        }
                    }

                    if (tkType == TokenType.symbol_colon)
                    {
                        if (ukagakaDatas.ContainsKey(previousToken.Value))
                        {
                            currentUkagaka = previousToken.Value;
                            currentUkagakaVar = definedName2variableNameTable_global[currentUkagaka];

                            inNekoExp = true;
                            lexer.SwitchTokenDefinationCollection(NekoExpressionDefinitions);
                        }
                    }

                }
                else
                {
                    AppendLine(lexer.Line);
                    lexer.NextLine();
                }

                previousToken = token;
                previousTkType = tkType;
            }
        }

        private void WriteHeadlines()
        {
            string headlines =
                @"
#coding=utf-8

import clr
clr.AddReference(ASSEMBLY_UkagakaW)

from UkagakaW.Core import GlobalEvent
from UkagakaW.Core import UkagakaEvent
import UkagakaW.Core.UkagakaW as UkagakaW
import UkagakaW.Core.Ukagaka as Ukagaka
import UkagakaW.Core.Animation as Animation

";

            scriptWriter.Write(headlines);
        }

        private void WritePreCompiles()
        {
            scriptWriter.Write
                (@"
import clr
clr.AddReference(ASSEMBLY_UkagakaW)

import UkagakaW.Shiori.NekoCompiler as NekoCompiler
");
            scriptWriter.WriteLine();
            scriptWriter.WriteLine("#COMPILER GENERATED PRE_COMPILE FUNCTION");
            scriptWriter.WriteLine("def PRE_COMPILE():");
            foreach(string ln in preCompiles)
            {
                scriptWriter.WriteLine("   " + ln);
            }
            scriptWriter.WriteLine("   pass");

            preCompiles.Clear();
        }

        private void WriteScript()
        {
            
            scriptWriter.Write(compiledPythonScript.ToString());

            compiledPythonScript.Clear();
        }

        private void WriteTails()
        {
            scriptWriter.WriteLine();
            foreach(string ln in tailLines)
            {
                scriptWriter.WriteLine(ln);
            }

            scriptWriter.WriteLine("#END OF THE SCRIPT ");

            scriptWriter.Close();
        }

        private string ConvertGlobalAction(string action, params string[] parameters)
        {
            ActionFunc func = globalActions[action];

            int parLength = parameters.Length;

            if (func.parameters != null)
            {
                if (func.parameters.Length > 0)  // This action have all or part of constant parameter
                {
                    if (parLength == 0)  // No parameter Given In using of action node
                    {
                        return String.Format("{0}({1})", func.function, func.GetParameterListString());
                    }
                    else
                    {
                        throw new Exception("parameter already given, do not need to provide any parameter here.");
                    }
                }
                else
                {
                    if (parameters.Length > 0)
                    {
                        return String.Format("{0}({1})", func.function, ActionFunc.GetParameterListString(parameters));
                    }
                    else
                    {
                        return String.Format("{0}()", func.action);
                    }
                }
            }
            else
            {
                throw new ArgumentException("Failed to load arguments information in action node function");
            }
        }

        private string ConvertUkagakaAction(string ukagakaScriptID, string action, params string[] parameters)
        {
            var uwData = ukagakaDatas[ukagakaScriptID];
            ActionFunc func = uwData.privateActions[action];

            string ukagakaVarName = definedName2variableNameTable_global[ukagakaScriptID];
            int parLength = parameters.Length;

            if (func.parameters != null)
            {
                if (func.parameters.Length > 0)  // This action have all or part of constant parameter
                {
                    if (parLength == 0)  // No parameter Given In using of action node
                    {
                        return String.Format("{0}.{1}({2})", ukagakaVarName, func.function, func.GetParameterListString());
                    }
                    else
                    {
                        throw new Exception("parameter already given, do not need to provide any parameter here.");
                    }
                }
                else
                {
                    if (parameters.Length > 0)
                    {
                        return String.Format("{0}.{1}({2})", ukagakaVarName, func.function, ActionFunc.GetParameterListString(parameters));
                    }
                    else
                    {
                        return String.Format("{0}.{1}()", ukagakaVarName, func.action);
                    }
                }
            }
            else
            {
                throw new ArgumentException("Failed to load arguments information in action node function");
            }
        }

        public void RegisterGlobalAction(string actionName, string function, params string[] parameters)
        {
            globalActions.Add(actionName, new ActionFunc() {
                action = actionName,
                function = function,
                parameters = parameters
            });
        }

        public void RegisterUkagakaAction(string ukagakaID, string actionName, string func, params string[] pars)
        {
            ukagakaDatas[ukagakaID].privateActions.Add(
                actionName,
                new ActionFunc() {
                    action = actionName,
                    function = func,
                    parameters = pars
                });
        }

        private void AddLine2PreCompile(string ln)
        {
            preCompiles.Add(ln);
        }

        private void AddLine2TailOfFile(string ln)
        {
            tailLines.Add(ln);
        }

        private void AppendLine(string ln)
        {
            compiledPythonScript.Append(' ', lexer.IndentLevel);
            compiledPythonScript.AppendLine(ln);
        }


        private string DefineUkagaka(string tkName, string tkUkagakaID)
        {
            if (!ukagakaDatas.ContainsKey(tkName))
            {
                string ukagakaVarName = "uk_" + tkName;
                this.definedName2variableNameTable_global[tkName] = ukagakaVarName;

                this.ukagakaDatas[tkName] = new UkagakaPrivateScriptData();

                return String.Format("{0} = UkagakaW.GetUkagaka({1})", ukagakaVarName, tkUkagakaID);
            }
            else
            {
                return String.Format("#[Error] There is already a ukagaka with name: '{0}' defined", tkName);
            }
        }

        private string DefineUkagakaAnimation(string tkName, string tkAnimTag, string tkUkagaka)
        {

            if (!this.definedName2variableNameTable_global.ContainsKey(tkAnimTag))
            {
                string animVarName = "anim_" + tkName;
                string ukagakaVarName = this.definedName2variableNameTable_global[tkUkagaka];

                this.definedName2variableNameTable_global.Add(tkName, animVarName);

                string ukaVarName = definedName2variableNameTable_global[tkUkagaka];
                AddLine2PreCompile(String.Format("NekoCompiler.Current.RegisterUkagakaAction('{0}', '{1}', 'Express', '{2}')", tkUkagaka, tkName, animVarName));

                return String.Format("{0} = {1}.GetAnimation({2})", animVarName, ukagakaVarName, tkAnimTag);
            }
            else
            {
                return String.Format("#[Error] There is already an anim for {0} with name: '{1}' defined", tkUkagaka, tkName);
            }
        }

        private string DefineGlobalAction(string tkName, string tkActionFunction, params string[] tkParameters)
        {
            if (!globalActions.ContainsKey(tkName))
            {
                this.AddLine2PreCompile(String.Format("NekoCompiler.Current.RegisterGlobalAction('{0}', '{1}', '{2}')", tkName, tkActionFunction, ActionFunc.GetParameterListString(tkParameters)));
                return "#Action {0} Registered in PreCompile function";
            }
            else
            {
                return String.Format("#[Error] There is already a global action with name: '{0}' defined", tkName);
            }
        }

        private string DefineUkagakaAction(string tkUkagaka, string tkName, string tkActionFunction, params string[] tkParameters)
        {
            if (!globalActions.ContainsKey(tkName))
            {
                this.AddLine2PreCompile(String.Format("NekoCompiler.Current.RegisterUkagakaAction('{0}', '{1}', '{2}', '{3}')", tkUkagaka, tkName, tkActionFunction, ActionFunc.GetParameterListString(tkParameters)));
                return "#Action {0} Registered in PreCompile function";
            }
            else
            {
                return String.Format("#[Error] There is already an ukagaka action for {0} with name: '{1}' defined", tkUkagaka, tkName);
            }
        }

        private string DefineGlobalEvent(string tkEventID, string tkEventFunction)
        {
            if (!globalEvents.ContainsKey(tkEventID))
            {
                this.AddLine2TailOfFile(String.Format("UkagakaW.RegisterGlobalEvent(GlobalEvent.{0}, {1})", tkEventID, tkEventFunction));
                return "#Event defined at end of the file";
            }
            else
            {
                return String.Format("#[Error] There is already an event with ID: '{0}' defined", tkEventID);
            }
        }

        private string DefineUkagakaEvent(string tkUkagaka, string tkEventID, string tkEventFunction)
        {
            string ukVarName = definedName2variableNameTable_global[tkUkagaka];
            if (!ukagakaDatas[tkUkagaka].privateEvents.ContainsKey(tkEventID))
            {
                this.AddLine2TailOfFile(String.Format("{0}.RegisterUkagakaEvent(UkagakaEvent.{1}, {2})", ukVarName, tkEventID, tkEventFunction));
                return "#Event defined at end of the file";
            }
            else
            {
                return String.Format("#[Error] There is already an event for {0} with ID: '{1}' defined", tkUkagaka, tkEventID);
            }
        }
    }

    interface IMatcher
    {
        int Match(string str);

        string Value { get; }
    }

    class RegexMatcher : IMatcher
    {
        protected readonly Regex regex;
        protected GroupCollection pars;
        public RegexMatcher(Regex regex) => this.regex = regex;  //^{0}

        public string Value
        {
            get
            {
                if (pars.Count > 1)
                {
                    return pars[1].Value;
                }
                else
                {
                    return pars[0].Value;
                }
            }
        }

        public virtual int Match(string text)
        {
            var m = regex.Match(text);
            if (m.Success) {
                pars = m.Groups;
                return m.Length;
            }
            else
            {
                return 0;
            }
        }
        public override string ToString() => regex.ToString();
    }

    class KeywordMatcher : IMatcher
    {

        string word;

        public KeywordMatcher(string word)
        {
            this.word = word;
            Value = word;
        }

        public string Value
        {
            get; private set;
        }

        public int Match(string str)
        {
            int i = 0;
            foreach(char c in str)
            {
                if (c == ' ')
                {
                    i++;
                    continue;
                }
                else
                    break;
            }

            int len = i + word.Length;

            if (len <= str.Length)
            {
                string s = str.Substring(i, word.Length);
                if(s == word)
                {
                    if (len < str.Length)
                    {
                        if (str[len] == ' ')
                            return i + word.Length;
                    }
                    else if (len == str.Length)
                        return i + word.Length;
                }
            }

            return -1;

        }
    }

    class SymbolMatcher : IMatcher
    {

        char symbol;

        public SymbolMatcher(char syb)
        {
            this.symbol = syb;

            Value = symbol.ToString();
        }

        public string Value
        {
            get; private set;
        }

        public int Match(string str)
        {
            if (str.Length > 0)
            {
                int i = 0;
                foreach (char c in str)
                {
                    if (c == ' ')
                    {
                        i++;
                        continue;
                    }
                    else
                        break;
                }

                if (i > str.Length - 1)
                    return -1;

                if (str[i] == symbol)
                    return i + 1;
                else
                    return -1;
            }
            else
            {
                return -1;
            }
        }
    }

    class GeneralFieldMatcher : IMatcher
    {
        public string Value { get; private set; }

        private char[] symbols = { '@', ':', '"', '\'', '$', '(', ')', '<', '>', '=', ' ' };

        private bool strict;

        public GeneralFieldMatcher(bool strict = false)
        {
            this.strict = strict;
        }

        public int Match(string str)
        {
            if (!strict)
            {
                bool start = false;

                int i = 0;
                int j = 0;
                foreach (char c in str)
                {
                    if (!start)
                    {
                        if (c == ' ')
                        {
                            i++;
                            continue;
                        }
                        else
                        {
                            j = i;
                            start = true;
                        }
                    }

                    if (start)
                    {
                        if (symbols.Contains(c) || i == str.Length - 1)
                        {
                            //still need to check
                            Value = str.Substring(j, i == j ? 1 : i - j);
                            return i == j ? i + 1 : i;
                        }
                        else
                        {
                            i++;
                            continue;
                        }
                    }
                }
                return -1;
            }
            else
            {
                int i = 0;
                foreach(char c in str)
                {
                    if ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122))
                        i++;
                    else
                        break;
                }
                Value = i > 0 ? str.Substring(0, i) : "";
                return i;
            }
        }
    }

    class TextMatcher : IMatcher
    {
        char[] sensitiveSymbols;

        public TextMatcher(char[] unexpectSymbols)
        {
            sensitiveSymbols = unexpectSymbols;
        }

        public string Value {
            get; private set;
        }

        public int Match(string str)
        {
            if(str.Length == 0 || str == "")
            {
                return -1;
            }

            bool start = false;
            int i = 0;
            int j = 0;
            int k = 0;
            foreach(char c in str)
            {
                if (!start) {
                    if (c != ' ')
                    {
                        start = true;
                        j = i;
                    }
                }

                if (start)
                {
                    if(c == ' ')
                    {
                        k++;
                    }
                    else
                    {
                        k = 0;
                    }
                    if (sensitiveSymbols.Contains<char>(c))
                    {
                        if (i == j)
                            return 0;
                        if (k > 0)
                        {
                            Value = str.Substring(j, i - j - k);
                        }
                        else
                        {
                            Value = str.Substring(j, i - j);
                        }
                        return i;
                    }
                }
                i++;
            }

            if (i == j)
                return 0;
            if (k > 0)
            {
                Value = str.Substring(j, i - j - k);
            }
            else
            {
                Value = str.Substring(j, i - j);
            }
            return i;
        }
    }

    sealed class TokenDefinition
    {
        public readonly IMatcher Matcher;
        public readonly TokenType Token;

        public TokenDefinition(IMatcher matcher, TokenType token)
        {
            this.Matcher = matcher;
            this.Token = token;
        }
        
        public TokenDefinition(Regex regex, TokenType token) : this(new RegexMatcher(regex), token) { }

        public TokenDefinition(string word, TokenType token) : this(new KeywordMatcher(word), token) { }

        public TokenDefinition(char symbol, TokenType token) : this(new SymbolMatcher(symbol), token) { }

    }

    sealed class Lexer : IDisposable
    {
        private readonly TextReader reader;
        private TokenDefinition[] tokenDefinitions;

        //private string content;

        private string lineRemaining;

        public Lexer(TextReader reader, TokenDefinition[] tokenDefinitions)
        {
            this.reader = reader;
            this.tokenDefinitions = tokenDefinitions;
            NextLine();
        }

        public void SwitchTokenDefinationCollection(TokenDefinition[] collections)
        {
            this.tokenDefinitions = collections;
        }

        public void NextLine()
        {
            do
            {
                lineRemaining = reader.ReadLine();
                Line = lineRemaining;
                ++LineNumber;
                Position = 0;
            } while (lineRemaining != null && lineRemaining.Length == 0);
        }

        public bool Next()
        {
            if (lineRemaining == null)
                return false;
            foreach (var def in tokenDefinitions)
            {
                if (lineRemaining.Length == 0)
                {
                    Token = TokenType.EOL;
                    TokenContents = "";
                    NextLine();
                    return true;
                }
                var matched = def.Matcher.Match(lineRemaining);
                if (matched > 0)
                {
                    Position += matched;
                    Token = def.Token;
                    TokenContents = def.Matcher.Value;  //lineRemaining.Substring(0, matched);
                    lineRemaining = lineRemaining.Substring(matched);

                    return true;
                }
            }

            throw new Exception(string.Format("Unable to match against any tokens at line {0} position {1} \"{2}\"",
                                              LineNumber, Position, lineRemaining));
        }

        public void RefreshReader()
        {
            reader.Close();
        }

        public string TokenContents
        {
            get;
            private set;

            //get
            //{
            //    return content;
            //}

            //private set
            //{
            //    int i = 0;
            //    foreach (char c in value)
            //    {
            //        if (c == ' ')
            //        {
            //            i++;
            //            continue;
            //        }
            //        else
            //            break;
            //    }

            //    content = value.Substring(i);
            //}
        }

        public TokenType Token { get; private set; }
        public string Line { get; private set; }
        public int LineNumber { get; private set; }
        public int Position { get; private set; }

        public int IndentLevel
        {
            get
            {
                return Line.Length - Line.TrimStart().Length;
            }
        }

        public void Dispose() => reader.Dispose();
    }
}
