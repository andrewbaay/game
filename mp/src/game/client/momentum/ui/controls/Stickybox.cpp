#include "cbase.h"

#include "Stickybox.h"

#include "iclientmode.h"

#include <vgui/ISurface.h>
#include <vgui_controls/AnimationController.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

Stickybox::Stickybox(Panel *pParent, const char *pElementName, Color *pAnimationColor, const char *pszAnimationName)
    : EditablePanel(pParent, pElementName), m_pAnimationColor(pAnimationColor),
    m_pszAnimationName(pszAnimationName), m_bProcessed(false)
{
}

void Stickybox::Process(StickyboxProc_t state)
{
    switch (state)
    {
    case STICKYBOX_PROCTYPE_NOSTICKY:
        ProcessNoSticky();
        break;
    case STICKYBOX_PROCTYPE_ACTIVE:
        ProcessActive();
        break;
    case STICKYBOX_PROCTYPE_DISABLED:
        ProcessDisabled();
        break;
    }
}

void Stickybox::ProcessNoSticky() 
{
    if (!m_bProcessed)
        return;

    StopAnimation();
    *m_pAnimationColor = m_NoStickyColor;
    SetBgColor(m_NoStickyColor);
    m_bProcessed = false;
}

void Stickybox::ProcessActive() 
{
    if (m_bProcessed)
    {
        SetBgColor(*m_pAnimationColor);
    }
    else
    {
        *m_pAnimationColor = m_PreArmColor;
        StartAnimation();
        m_bProcessed = true;
    }
}

bool Stickybox::StartAnimation()
{
    if (!m_pszAnimationName || !m_pAnimationColor)
        return false;

    return g_pClientMode->GetViewportAnimationController()->StartAnimationSequence(m_pszAnimationName);
}

bool Stickybox::StopAnimation() 
{
    if (!m_pszAnimationName || !m_pAnimationColor)
        return false;

    return g_pClientMode->GetViewportAnimationController()->StopAnimationSequence(GetParent(), m_pszAnimationName);
}

void Stickybox::SwapAnimations(Stickybox *pOther) 
{
    V_swap(m_pAnimationColor, pOther->m_pAnimationColor);
    V_swap(m_pszAnimationName, pOther->m_pszAnimationName);
}

void Stickybox::ApplySchemeSettings(IScheme *pScheme) 
{
    BaseClass::ApplySchemeSettings(pScheme);

    *m_pAnimationColor = m_NoStickyColor;
    SetBgColor(m_NoStickyColor);
}
