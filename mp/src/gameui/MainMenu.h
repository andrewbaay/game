#pragma once

#include "vgui2d/panel2d.h"
#include "igameevents.h"

#include "button_mainmenu.h"

enum SortFlags_t
{
    FL_SORT_SHARED = 1 << 0,
    FL_SORT_INGAME = 1 << 1,
    FL_SORT_MENU = 1 << 2
};

class MainMenu : public Panel2D, public IGameEventListener2
{
    DECLARE_CLASS_SIMPLE(MainMenu, Panel2D);

    MainMenu(Panel *parent);
    ~MainMenu();

    void OnThink() OVERRIDE;
    void OnTick() OVERRIDE;
    bool IsVisible() OVERRIDE;
    void OnCommand(char const *cmd) OVERRIDE;

    void FireGameEvent(IGameEvent* event) OVERRIDE;

    void OnScreenSizeChanged(int oldwide, int oldtall) OVERRIDE;

    void ApplySchemeSettings(vgui::IScheme *pScheme) OVERRIDE;
    void CreateMenu();
    void DrawMainMenu();
    void DrawLogo();
    void CheckVersion();
    void Paint() OVERRIDE;


    void Activate()
    {
        MoveToFront();
        SetVisible(true);
        SetEnabled(true);
    }

private:
    CUtlVector<Button_MainMenu *> m_pButtons;

    // Our own buttons...
    Button_MainMenu *m_pButtonLobby;
    Button_MainMenu *m_pButtonInviteFriends;

    // Pointers to main menu buttons...

    Button_MainMenu *m_pButtonSpectate;
    bool m_bIsSpectating, m_bInGame, m_bInLobby;


    char m_pszMenuOpenSound[MAX_PATH];
    char m_pszMenuCloseSound[MAX_PATH];

    bool m_bFocused;
    wchar_t *m_logoLeft;
    wchar_t *m_logoRight;

    float m_fButtonsSpace;

    float m_fButtonsOffsetX;
    float m_fButtonsOffsetY;

    float m_fLogoOffsetX;
    float m_fLogoOffsetY;

    bool m_bLogoPlayerCount;
    bool m_bLogoText;
    vgui::ImagePanel *m_pLogoImage;
    bool m_bLogoAttachToMenu;

    Color m_cLogoLeft;
    Color m_cLogoRight;
    Color m_cLogoPlayerCount;

    vgui::HFont m_fLogoPlayerCount;
    vgui::HFont m_fLogoFont;
    int m_nSortFlags;
    bool m_bNeedSort;

    vgui::Label *m_pVersionLabel;
};
