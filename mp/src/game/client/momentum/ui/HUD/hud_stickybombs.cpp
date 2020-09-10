#include "cbase.h"

#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/AnimationController.h>
#include <vgui_controls/Button.h>

#include "c_mom_player.h"
#include "hudelement.h"
#include "iclientmode.h"
#include "mom_system_gamemode.h"
#include "weapon/weapon_mom_stickybomblauncher.h"
#include "weapon/weapon_shareddefs.h"

#include "tier0/memdbgon.h"

using namespace vgui;

static MAKE_TOGGLE_CONVAR(mom_hud_sj_stickycount_enable, "1", FCVAR_ARCHIVE, "Toggles the stickybomb counter.\n");

struct Stickybox_t
{
    Button *button;
    Color *color;
    const char *animationSeq;
    bool flag;
};

class CHudStickybombs : public CHudElement, public EditablePanel
{
    DECLARE_CLASS_SIMPLE(CHudStickybombs, EditablePanel);

  public:
    CHudStickybombs(const char *pElementName);

    bool ShouldDraw() override;
    void OnThink() override;
    void ApplySchemeSettings(IScheme *pScheme) override;

  private:
    Stickybox_t m_Stickyboxes[MOM_WEAPON_STICKYBOMB_COUNT];

    CPanelAnimationVar(Color, m_NoStickyColor, "NoStickyColor", "BlackHO");
    CPanelAnimationVar(Color, m_PreArmColor, "PreArmColor", "BlackHO");
    CPanelAnimationVar(Color, m_BgColor, "BgColor", "Blank");
    CPanelAnimationVar(Color, m_FirstStickyColor, "FirstStickyColor", "BlackHO");
    CPanelAnimationVar(Color, m_SecondStickyColor, "SecondStickyColor", "BlackHO");
    CPanelAnimationVar(Color, m_ThirdStickyColor, "ThirdStickyColor", "BlackHO");
};

DECLARE_HUDELEMENT(CHudStickybombs);

CHudStickybombs::CHudStickybombs(const char *pElementName)
    : CHudElement(pElementName), BaseClass(g_pClientMode->GetViewport(), "CHudStickybombs")
{
    SetHiddenBits(HIDEHUD_LEADERBOARDS);

    m_Stickyboxes[0] = { new Button(this, "FirstStickyState", ""), &m_FirstStickyColor, "FirstStickyArm", false };
    m_Stickyboxes[1] = { new Button(this, "SecondStickyState", ""), &m_SecondStickyColor, "SecondStickyArm", false };
    m_Stickyboxes[2] = { new Button(this, "ThirdStickyState", ""), &m_ThirdStickyColor, "ThirdStickyArm", false };

    LoadControlSettings("resource/ui/HudStickybombs.res");
}

bool CHudStickybombs::ShouldDraw()
{
    if (!mom_hud_sj_stickycount_enable.GetBool() || !g_pGameModeSystem->GameModeIs(GAMEMODE_SJ))
        return false;

    C_MomentumPlayer *pPlayer = C_MomentumPlayer::GetLocalMomPlayer();
    if (!pPlayer || !pPlayer->IsAlive())
        return false;

    return CHudElement::ShouldDraw();
}

void CHudStickybombs::OnThink()
{
    C_MomentumPlayer *pPlayer = C_MomentumPlayer::GetLocalMomPlayer();
    if (!pPlayer)
        return;

    const auto pStickyPtr = pPlayer->GetWeapon(WEAPON_STICKYLAUNCHER);
    if (!pStickyPtr)
        return;

    const auto pStickylauncher = static_cast<CMomentumStickybombLauncher *>(pStickyPtr);
    const auto iStickybombs = pStickylauncher->GetStickybombCount();

    for (int i = 0; i < MOM_WEAPON_STICKYBOMB_COUNT; i++)
    {
        if (i < iStickybombs)
        {
            if (m_Stickyboxes[i].flag)
            {
                *m_Stickyboxes[i].color = m_PreArmColor;
                g_pClientMode->GetViewportAnimationController()->StartAnimationSequence(m_Stickyboxes[i].animationSeq);
                m_Stickyboxes[i].flag = false;
            }
            m_Stickyboxes[i].button->SetBgColor(*m_Stickyboxes[i].color);
        }
        else if (!m_Stickyboxes[i].flag)
        {
            if (i > 0)
            {
                // moving into left-most box, so swap color animation to keep the current fade-in
                V_swap(m_Stickyboxes[i].color, m_Stickyboxes[0].color);
                V_swap(m_Stickyboxes[i].animationSeq, m_Stickyboxes[0].animationSeq);
            }
            *m_Stickyboxes[i].color = m_NoStickyColor;
            m_Stickyboxes[i].button->SetBgColor(m_NoStickyColor);
            m_Stickyboxes[i].flag = true;
        }
    }
}

void CHudStickybombs::ApplySchemeSettings(IScheme* pScheme)
{
    BaseClass::ApplySchemeSettings(pScheme);

    for (int i=0; i < MOM_WEAPON_STICKYBOMB_COUNT; i++)
    {
        m_Stickyboxes[i].button->SetBgColor(m_NoStickyColor);
    }

    SetBgColor(m_BgColor);
}
