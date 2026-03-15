#include <Geode/Geode.hpp>
#include <Geode/modify/LevelCell.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/FLAlertLayer.hpp>

using namespace geode::prelude;

namespace {
    constexpr int kLevelLengthXL = 4;

    void addBlockOverlay(CCNode* host, bool isXL, bool isCompact) {
        if (!host) return;

        auto blur = CCLayerColor::create({0, 0, 0, 110});
        blur->setContentSize({356.f, 90.f});
        if(isCompact) {
            blur->setContentHeight(50.f);
        }
        blur->setZOrder(1);
        host->addChild(blur);

        auto x = CCLabelBMFont::create("X", "chatFont.fnt");
        x->setAnchorPoint({0.5f, 0.5f});
        x->setPosition({
            blur->getPositionX() + 173.f,
            blur->getPositionY() + blur->getContentHeight() / 2 
        });
        x->setColor({255, 0, 0});
        x->setZOrder(2);

        // super unnecessary codes
        if (isCompact) {
            x->setScaleX(8.f);
            x->setScaleY(2.f);
        } else {
            x->setScaleX(20.f);
            x->setScaleY(4.f);
        }

        blur->addChild(x);
    }
}

class $modify(noPlatAndXL, LevelCell) {
    void loadFromLevel(GJGameLevel* level) {
        LevelCell::loadFromLevel(level);
        if (!level) return;

        bool hideXl = Mod::get()->getSettingValue<bool>("hide_XL");
        bool hidePlat = Mod::get()->getSettingValue<bool>("hide_plat");

        bool isPlat = level->isPlatformer();
        bool isXL = level->m_levelLength == kLevelLengthXL;

        if ((hidePlat && isPlat) || (hideXl && isXL)) {
            addBlockOverlay(this, isXL, m_compactView);
        }
    }
};

class $modify(notifyPlatAndXL, LevelInfoLayer) {
    struct Fields {
        bool warnIsXL = false;
        bool warnIsPlat = false;
    };

    void onWarnButton(CCObject*) {
        std::string content;

        if (m_fields->warnIsPlat) {
            content = "You are playing <cr>Platformer</c> level.\nYou might ruin your day.";
        } else if (m_fields->warnIsXL) {
            content = "You are playing <cr>XL</c> level.\nYou might ruin your day.";
        } else {
            return;
        }

        FLAlertLayer::create("Warning", content, "ok")->show();
    }

    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;
        if (!level) return true;

        bool warnXl   = Mod::get()->getSettingValue<bool>("hide_XL");
        bool warnPlat = Mod::get()->getSettingValue<bool>("hide_plat");

        m_fields->warnIsPlat = warnPlat && level->isPlatformer();
        m_fields->warnIsXL   = warnXl && (level->m_levelLength == 4);

        if (!m_fields->warnIsPlat && !m_fields->warnIsXL) return true;

        auto playMenu = m_playBtnMenu;
        auto playBtn  = playMenu->getChildByID("play-button");
        if (!playMenu || !playBtn) return true;

        auto spr = CCSprite::createWithSpriteFrameName("exMark_001.png");
        if (!spr) return true;

        auto btn = CCMenuItemSpriteExtra::create(
            spr,
            this,
            menu_selector(notifyPlatAndXL::onWarnButton)
        );

        btn->setPosition(playBtn->getPosition() + CCPoint{30.f, 15.f});
        btn->setZOrder(10);
        playMenu->addChild(btn);

        return true;
    }
};