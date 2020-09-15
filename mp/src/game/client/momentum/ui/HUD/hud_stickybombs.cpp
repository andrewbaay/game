#include "cbase.h"

#include <vgui_controls/EditablePanel.h>

#include "c_mom_player.h"
#include "hudelement.h"
#include "iclientmode.h"
#include "mom_system_gamemode.h"
#include "weapon/weapon_mom_stickybomblauncher.h"
#include "weapon/weapon_shareddefs.h"
#include "controls/Stickybox.h"

#include "tier0/memdbgon.h"

using namespace vgui;

static MAKE_TOGGLE_CONVAR(mom_hud_sj_stickycount_enable, "1", FCVAR_ARCHIVE, "Toggles the stickybomb counter.\n");

class CHudStickybombs : public CHudElement, public EditablePanel
{
    DECLARE_CLASS_SIMPLE(CHudStickybombs, EditablePanel);

  public:
    CHudStickybombs(const char *pElementName);

    bool ShouldDraw() override;
    void OnThink() override;
    void ApplySchemeSettings(IScheme *pScheme) override;
    void PerformLayout() override;

  private:
    CUtlVector<Stickybox*> m_Stickyboxes;

    CPanelAnimationVar(int, m_iBoxDimension, "BoxDimension", "8");
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

    m_Stickyboxes.EnsureCount(MOM_WEAPON_STICKYBOMB_COUNT);
    m_Stickyboxes[0] = new Stickybox(this, "FirstStickybox", &m_FirstStickyColor, "FirstStickyArm");
    m_Stickyboxes[1] = new Stickybox(this, "SecondStickybox", &m_SecondStickyColor, "SecondStickyArm");
    m_Stickyboxes[2] = new Stickybox(this, "ThirdStickybox", &m_ThirdStickyColor, "ThirdStickyArm");

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
            bool bCanExplode = true;
            const auto pSticky = pStickylauncher->GetStickyByCount(i);
            if (pSticky)
            {
                bCanExplode = pSticky->CanExplode();
            }
            
            m_Stickyboxes[i]->Process(bCanExplode ? STICKYBOX_PROCTYPE_ACTIVE : STICKYBOX_PROCTYPE_DISABLED);
        }
        else
        {
            m_Stickyboxes[i]->Process(STICKYBOX_PROCTYPE_NOSTICKY);
        }
        
        if (m_Stickyboxes[i]->IsProcessed() && i > 0 && i >= iStickybombs)
        {
            // moving into left-most box, so swap color animation to keep the current fade-in animation
            m_Stickyboxes[i]->SwapAnimations(m_Stickyboxes[0]);
        }
    }
}

void CHudStickybombs::ApplySchemeSettings(IScheme* pScheme)
{
    BaseClass::ApplySchemeSettings(pScheme);
    SetBgColor(m_BgColor);
}

void CHudStickybombs::PerformLayout()
{
    BaseClass::PerformLayout();

    int iSpacePerBox = GetWide() / MOM_WEAPON_STICKYBOMB_COUNT;
    int iXPosAcc = 0;
    for (int i = 0; i < MOM_WEAPON_STICKYBOMB_COUNT; i++)
    {
        int iScaledBoxDimension = GetScaledVal(m_iBoxDimension);

        m_Stickyboxes[i]->SetWide(iScaledBoxDimension);
        m_Stickyboxes[i]->SetTall(iScaledBoxDimension);
        m_Stickyboxes[i]->SetPos(iXPosAcc + (iSpacePerBox - iScaledBoxDimension) / 2, GetTall() / 2 - iScaledBoxDimension / 2);

        iXPosAcc += iSpacePerBox;
    }
}
