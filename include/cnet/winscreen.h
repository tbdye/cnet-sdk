#ifndef CNET_WINSCREEN_H
#define CNET_WINSCREEN_H

struct WinStruct
	{
	struct Window *Window;
	struct Gadget *GList;
	struct Menu *Menus;
	struct IntuiText *WinText;
	struct Requester *BlockRequester;
	LONG NW_LeftEdge;
	LONG NW_TopEdge;
	LONG NW_Width;
	LONG NW_Height;
	LONG NW_MinWidth;
	LONG NW_MinHeight;
	LONG NW_MaxWidth;
	LONG NW_MaxHeight;
	WORD NW_TotalGads;
	LONG BlockCnt;
	ULONG WS_Flags;
	BOOL NW_Reset;
	} __attribute__((packed));

struct WindowFont
	{
	struct TextFont *wf_CurrentFont;		// pointer to the currently opened window font, or NULL if none
	struct TextAttr wf_TextAttr;			// font spec
	BOOL Changed;								// true if changed by user
	} __attribute__((packed));

#endif /* CNET_WINSCREEN_H */
