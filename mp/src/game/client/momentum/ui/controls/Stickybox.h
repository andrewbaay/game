#pragma once

#include <vgui_controls/EditablePanel.h>

namespace vgui
{

enum StickyboxProc_t
{
    STICKYBOX_PROCTYPE_NOSTICKY = 0,
    STICKYBOX_PROCTYPE_ACTIVE,
    STICKYBOX_PROCTYPE_DISABLED,

    STICKYBOX_PROCTYPE_COUNT
};

class Stickybox : public EditablePanel
{
    DECLARE_CLASS_SIMPLE(Stickybox, EditablePanel);

  public:
    Stickybox(Panel *pParent, const char *pElementName, Color *pAnimationColor, const char *pszAnimationName);

    bool IsProcessed() { return m_bProcessed; }

    void Process(StickyboxProc_t state);

    bool StartAnimation();
    bool StopAnimation();

    void SwapAnimations(Stickybox *pOther);

    void ApplySchemeSettings(IScheme *pScheme) override;

  private:
    void ProcessNoSticky();
    void ProcessActive();
    void ProcessDisabled() { SetBgColor(m_DisabledColor); }

    const char *m_pszAnimationName;
    Color *m_pAnimationColor;

    bool m_bProcessed;

    CPanelAnimationVar(Color, m_NoStickyColor, "NoStickyColor", "BlackHO");
    CPanelAnimationVar(Color, m_PreArmColor, "PreArmColor", "BlackHO");
    CPanelAnimationVar(Color, m_DisabledColor, "DisabledColor", "MomentumRed");
};

} // namespace vgui
