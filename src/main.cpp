#include <Geode/Geode.hpp>
#include <Geode/modify/LevelCell.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class $modify(noPlatAndXL, LevelCell) {
	void loadFromLevel(GJGameLevel* level) {
		LevelCell::loadFromLevel(level);

		if (!level) return;

		bool hideXl = Mod::get()->getSettingValue<bool>("hide_XL");
		bool hidePlat = Mod::get()->getSettingValue<bool>("hide_plat");

		if (hidePlat && level->isPlatformer()) {

			auto spr = CCLabelBMFont::create("X", "chatFont.fnt");
			spr->setPosition({
				170,
				45
			});
			spr->setScaleX(20);
			spr->setScaleY(4);
			spr->setZOrder(100);
			spr->setColor({255,0,0});

			this->addChild(spr);   
		}

		if (hideXl && level->m_levelLength == 4) {

			auto spr = CCLabelBMFont::create("X", "chatFont.fnt");
			spr->setPosition({
				170,
				45
			});
			spr->setScaleX(15);
			spr->setScaleY(4);
			spr->setZOrder(100);
			spr->setColor({255,0,0});

			this->addChild(spr);   
		}
	}
};