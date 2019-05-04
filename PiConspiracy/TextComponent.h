//
//  TextComponent.h
//  PiEngine
//
//  Created by PiConspiracy on 12/01/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

class TextRenderer;
class TextComponent : public Component
{
	COMPONENT_DECLARE(TextComponent);

public:
							~TextComponent(void);

	void				    Initialize() override final;
	void				    Update(float a_dTime) override final;
	void				    Serialize(const json&) override final;
	void				    HandleEvent(Event* a_pEvent) override final;

	virtual void		    Deactivate(void) override;

	virtual void		    OverrideParams(const json& override, float fOffset) override;

private:

	TextRenderer*			m_pTextRenderer;

	string					m_strText;
	string					m_strFontName;
	const GLchar*			m_strShaderName;
	uint32					m_uFontSize;
	float					m_fScale;
	Color3					m_vColor;
	vec2					m_vPosition;

};

