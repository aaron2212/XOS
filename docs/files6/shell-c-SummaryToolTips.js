NDSummary.OnToolTipsLoaded("File6:shell.c",{129:"<div class=\"NDToolTip TStruct LC\"><div class=\"TTSummary\">The command text, along with a function that is executed when the command is typed</div></div>",131:"<div class=\"NDToolTip TVariable LC\"><div id=\"NDPrototype131\" class=\"NDPrototype NoParameterForm\">fn_table_entry_t fn_table[]</div><div class=\"TTSummary\">Stores all the shell commands with their corresponding functions to be called when the command is entered</div></div>",133:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype133\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> (</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PNamePrefix first\">*</td><td class=\"PName last\">fn_lookup(char* fname)</td></tr></table></td><td class=\"PAfterParameters\">)()</td></tr></table></div><div class=\"TTSummary\">Searches for a valid command, and calls the function corresponding to it</div></div>",134:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype134\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> shell()</div><div class=\"TTSummary\">The shell that handles use input</div></div>",135:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype135\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">char</span>* read_line()</div><div class=\"TTSummary\">Read the inputted line</div></div>",136:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype136\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">char</span>** split_line(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">char</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">line</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Split the inputted line by <u>TOK DELIM</u></div></div>",137:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype137\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> xsh_help()</div><div class=\"TTSummary\">Print a help message displaying the list of available commands</div></div>",138:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Print the text after the <u>echo</u> command</div></div>",139:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype139\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> xsh_exit()</div><div class=\"TTSummary\">Exit the shell</div></div>",140:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype140\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> xsh_clear_screen()</div><div class=\"TTSummary\">Clear the terminal screen</div></div>",141:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype141\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> xsh_cat(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">char</span>&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">args</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Display the contents of a file</div></div>",142:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype142\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> xsh_touch(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">char</span>&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">args</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Create a new file</div></div>",143:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype143\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> xsh_cd(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">char</span>&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">args</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Change the current working directory</div></div>",144:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype144\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> xsh_ls(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">char</span>&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">args</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Display the contents of the current directory</div></div>",145:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype145\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> xsh_pwd()</div><div class=\"TTSummary\">Print the current working directory</div></div>",146:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype146\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> xsh_rm(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">char</span>&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">args</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Remove a file from the filesystem</div></div>",147:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype147\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> xsh_rmdir(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">char</span>&nbsp;</td><td class=\"PNamePrefix\">**</td><td class=\"PName last\">args</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Remove a directory and its contents from the filesystem</div></div>",148:"<div class=\"NDToolTip TFunction LC\"><div class=\"TTSummary\">Print the system\'s hostname</div></div>",149:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype149\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> xsh_login()</div><div class=\"TTSummary\">Login as a user using a username and password</div></div>",150:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype150\" class=\"NDPrototype NoParameterForm\"><span class=\"SHKeyword\">void</span> xsh_uptime()</div><div class=\"TTSummary\">Print the system uptime in seconds</div></div>"});