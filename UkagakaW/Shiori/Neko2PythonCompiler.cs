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

namespace UkagakaW.Shiori
{
    public enum ActionType
    {
        NativeFunction = 0,
        Animation = 1,
        None = 114514
    }

    public struct ActionFunc
    {
        ActionType type;
        string action;

        String nativeFunc;
        String animation;
    }

    public sealed class Neko2PythonCompiler
    {

        public StringBuilder compiledPythonScript;
        public string sourceNekoScript;

        private List<ActionFunc> actions_;

        private Dictionary<string, int> anims;
        private Dictionary<string, int> natFuncs;

        private List<string> ukagakaTags;

        public void ReadNekoScript(string path)
        {

        }

        public void Compile()
        {            
            
        }

        public ActionType CheckAcionType(string action)
        {
            if (natFuncs.ContainsKey(action))
            {
                return ActionType.NativeFunction;
            }
            else if(anims.ContainsKey(action))
            {
                return ActionType.Animation;
            }
            else
            {
                return (ActionType)114514;
            }
        }
    }

    interface IMatcher
    {
        int Match(string str);

        object Parameter { get; }
    }

    class RegexMatcher : IMatcher
    {
        protected readonly Regex regex;
        protected GroupCollection pars;
        public RegexMatcher(string regex) => this.regex = new Regex(string.Format("^{0}$", regex));

        public object Parameter
        {
            get
            {
                return pars;
            }
            protected set => pars = (GroupCollection)value;
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

    sealed class ActionMatcher : RegexMatcher
    {
        Neko2PythonCompiler compiler_;
        /*<sammary>
        * example:
        * @smile action = "smile" par0 = null
        * @w25 action = "w" par0 = "25"
        *</sammary>
        */
        public ActionMatcher(Neko2PythonCompiler compiler) : base(@"@(?<action>[a-z]+)(?<par0>[0-9]{0-2})@?\s")
        {
            this.compiler_ = compiler;
        }

        public override int Match(string str)
        {
            var m = regex.Match(str);
            if (m.Success)
            {
                var act = m.Groups["action"].Value;
                ActionType type = compiler_.CheckAcionType(act);
                if (compiler_.CheckAcionType(act) != ActionType.None)
                {
                    Parameter = new Dictionary<string, object>() {
                            {"action", act  },
                            {"par0", m.Groups["par0"].Value },
                            {"type", type }
                        };
                }
                return str.Length;
            }
            return 0;
        }
    }

    sealed class PythonMatcher : IMatcher
    {
        public PythonMatcher()
        {

        }

        public object Parameter => throw new NotImplementedException();

        public int Match(string str)
        {
            Regex assignment = new Regex("^.+ = .+$");
            Regex methodDefine = new Regex("^def .+(.*):$");
            Regex importStatement = new Regex("^import +.+ +(as +.+)?$");
            Regex ifStatement = new Regex("if +.+:");
            Regex elseStatement = new Regex("else:");
            Regex forStatement = new Regex("(.+\\.)?.+\\(.*\\)");


            //TODO: match regex
            return 0;
        }
    }

    sealed class TokenDefinition
    {
        public readonly IMatcher Matcher;
        public readonly object Token;

        public TokenDefinition(string regex, object token)
        {
            this.Matcher = new RegexMatcher(regex);
            this.Token = token;
        }

        public TokenDefinition(Neko2PythonCompiler compiler, object token)
        {
            this.Matcher = new ActionMatcher(compiler);
            this.Token = token;
        }
    }

    sealed class Lexer : IDisposable
    {
        private readonly TextReader reader;
        private readonly TokenDefinition[] tokenDefinitions;

        private string lineRemaining;

        public Lexer(TextReader reader, TokenDefinition[] tokenDefinitions)
        {
            this.reader = reader;
            this.tokenDefinitions = tokenDefinitions;
            nextLine();
        }

        private void nextLine()
        {
            do
            {
                lineRemaining = reader.ReadLine();
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
                var matched = def.Matcher.Match(lineRemaining);
                if (matched > 0)
                {
                    Position += matched;
                    Token = def.Token;
                    TokenContents = lineRemaining.Substring(0, matched);
                    lineRemaining = lineRemaining.Substring(matched);
                    if (lineRemaining.Length == 0)
                        nextLine();

                    return true;
                }
            }
            throw new Exception(string.Format("Unable to match against any tokens at line {0} position {1} \"{2}\"",
                                              LineNumber, Position, lineRemaining));
        }

        public string TokenContents { get; private set; }
        public object Token { get; private set; }
        public int LineNumber { get; private set; }
        public int Position { get; private set; }

        public void Dispose() => reader.Dispose();
    }
}
