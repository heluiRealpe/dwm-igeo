/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=13" };
static const char dmenufont[]       = "monospace:size=14";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

void
togglemaximize(const Arg *arg)
{
    /*XEvent ev;*/
    /*XClientMessageEvent *cmev = &ev.xclient;*/

    if (!selmon->sel || selmon->sel->isfullscreen) /* no client selected or is fullscreen */
        return;
    if (selmon->sel->isfloating && selmon->sel->bw == 0) {
        /* if the window is already maximized, restore it */
        resize(selmon->sel, selmon->sel->oldx, selmon->sel->oldy,
               selmon->sel->oldw, selmon->sel->oldh, True);
        selmon->sel->bw = borderpx;
    } else {
        /* save the window's old dimensions */
        selmon->sel->oldx = selmon->sel->x;
        selmon->sel->oldy = selmon->sel->y;
        selmon->sel->oldw = selmon->sel->w;
        selmon->sel->oldh = selmon->sel->h;
        /* maximize the window */
        resize(selmon->sel, selmon->wx, selmon->wy,
               selmon->ww, selmon->wh, True);
        selmon->sel->bw = 0;
    }
    XSync(dpy, False);
}

void
resizefloating(const Arg *arg) {
    if (!selmon->sel || !selmon->sel->isfloating)
        return;
    resize(selmon->sel, selmon->sel->x, selmon->sel->y,
           selmon->sel->w + arg->i, selmon->sel->h + arg->i, True);
}

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6"
    /*, "7", "8", "9" */
};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* Function definitions */
void
movetagandfocus(const Arg *arg)
{
    if (!selmon->sel || !mons->next)
        return;
    tagmon(arg);
    focusmon(arg);
}

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *slockcmd[]  = { "slock", NULL };
static const char *firefoxcmd[]  = { "firefox", NULL };
static const char *thunarcmd[]  = { "thunar", NULL };
/*static const char *eclipsecmd[]  = { "eclipse", NULL };*/
static const char *tabbedcmd[] = { "tabbed", "-c", "-r", "2", "st", "-w", "'2'", NULL };
static const char *vol_up[]   = { "/bin/sh", "-c", "amixer set Master 5%+", NULL };
static const char *vol_down[] = { "/bin/sh", "-c", "amixer set Master 5%-", NULL };
static const char *play_stop_toggle[] = { "/bin/sh", "-c", "playerctl --player=spotify play-pause", NULL };
static const char *roficmd[] = { "/bin/sh", "-c", "rofi -show drun", NULL };
static const char *flameshotcmd[] = { "/bin/sh", "-c", "flameshot gui -r | xclip -selection clipboard -t image/png", NULL };
static const char *stscmd[] = { "/bin/sh", "-c", "/opt/sts4/SpringToolSuite4", NULL };


static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = roficmd } },
	// { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = tabbedcmd } },
	{ MODKEY|ShiftMask,             XK_l, spawn,               {.v = slockcmd } },
	{ MODKEY,                       XK_w, spawn,               {.v = firefoxcmd } },
	{ MODKEY,                       XK_e, spawn,               {.v = thunarcmd } },
	{ MODKEY,                       XK_r, spawn,               {.v = stscmd } },
	{ MODKEY,                       XK_u, spawn,               {.v = vol_up } },
	{ MODKEY,                       XK_y, spawn,               {.v = vol_down } },
	{ MODKEY,                       XK_o, spawn,               {.v = play_stop_toggle } },
	{ MODKEY,                       XK_s, spawn,              {.v = flameshotcmd } },
	{ MODKEY| ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
    { MODKEY,                       XK_x,   resizefloating,       {.i = 20 } }, /* Increase size */
    { MODKEY,                       XK_z,  resizefloating,       {.i = -20 } }, /* Decrease size */
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_m,      togglemaximize, {0} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ControlMask,           XK_comma,  movetagandfocus,{.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, movetagandfocus,{.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

