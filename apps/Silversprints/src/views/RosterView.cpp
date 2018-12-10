//
//  RosterView.cpp
//  SilverSprints
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#include "views/RosterView.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

RosterView::RosterView() : bVisible(false) {
    
}

void RosterView::setup(){
    mBg = gl::Texture::create( loadImage( loadAsset("img/bgGrad.png") ) );
    mCancelBtn = gl::Texture::create( loadImage( loadAsset("img/rosterCancel.png") ) );
    
    StateManager::instance().signalOnStateChange.connect( std::bind(&RosterView::onStateChange, this, std::placeholders::_1) );
    
    for( int i=0; i<4; i++){
        float yPos = 190 + 183 * i;
        
        CiTextField *tf = new CiTextField("Player "+to_string(i+1), Rectf(574, yPos, 574 + 910, yPos+150), ci::Font(loadAsset("fonts/UbuntuMono-R.ttf"), 75) );
        tf->mColorStroke = Model::instance().playerColors[i];
        tf->mColorFill = Model::instance().playerColors[i];
        tf->mColorText = Color::black();
        tf->padding = vec2(30,30);
        
        tf->bUseScissorTest = false;
        mPlayerNames.push_back(tf);

        
        CiTextField *tf2 = new CiTextField("0", Rectf(1498, yPos, 1800, yPos+150), ci::Font(loadAsset("fonts/UbuntuMono-R.ttf"), 75) );
        tf2->mColorStroke = Model::instance().playerColors[i];
        tf2->mColorFill = Model::instance().playerColors[i];
        tf2->mColorText = Color::black();
        tf2->padding = vec2(30,30);
        
        tf2->bUseScissorTest = false;
        mHandicaps.push_back(tf2);

        //mCancelRects.push_back( Rectf(1498, yPos, 1555, yPos+52) );
    }
    
    tFont = ci::gl::TextureFont::create( ci::Font(loadAsset("fonts/UbuntuMono-R.ttf"), 75) );
    
    ci::app::WindowRef win = getWindow();
    win->getSignalKeyDown().connect(std::bind(&RosterView::onKeyDown, this, std::placeholders::_1));
    win->getSignalMouseUp().connect(std::bind(&RosterView::onMouseUp, this, std::placeholders::_1));
}

void RosterView::onMouseUp( ci::app::MouseEvent event ) {
    
//    vec2 pos = Model::instance().localToGlobal( event.getPos() );
//    
//    for( int i=0; i<4; i++){
//        if( mCancelRects[i].contains(pos) ){
//            mPlayerNames[i]->disable();
//            break;
//        }
//    }
    
}

void RosterView::onKeyDown( ci::app::KeyEvent event ) {
    if( event.getCode() == KeyEvent::KEY_TAB ){
        int i=0;
        for( ; i<mPlayerNames.size(); i++ ){
            if( mPlayerNames[i]->isActive() ){
                mPlayerNames[i]->blur();
                
                if( i == mPlayerNames.size()-1 ){
                    mPlayerNames[0]->focus(true);
                }else{
                    mPlayerNames[i+1]->focus(true);
                }
                break;
            }
        }
        if( i == mPlayerNames.size() ){ // nothing selected. so select the first field
            mPlayerNames[0]->focus(true);
        }
    }
}

void RosterView::onStateChange(APP_STATE newState){
    if( newState == APP_STATE::ROSTER ){
        animateIn();
    }else{
        animateOut();
    }
}

void RosterView::animateIn(){
    if( bVisible ){
        return;
    }
    
    bVisible = true;
    for( auto it = mPlayerNames.begin(); it!=mPlayerNames.end(); ++it){
        (*it)->visible = true;
    }
    
    if (!Model::instance().getIsHandicapped())
        return;

    for (auto it = mHandicaps.begin(); it != mHandicaps.end(); ++it) {
        (*it)->visible = true;
    }
}

void RosterView::animateOut(){
    if(!bVisible){
        return;
    }
    
    for( int i=0; i<mPlayerNames.size(); i++ ){
        Model::instance().playerData[i]->player_name = mPlayerNames[i]->getText();
    }
    
    bVisible = false;
    for( auto it = mPlayerNames.begin(); it!=mPlayerNames.end(); ++it){
        (*it)->visible = false;
    }
    
    if (!Model::instance().getIsHandicapped())
        return;

    for (auto it = mHandicaps.begin(); it != mHandicaps.end(); ++it) {
        (*it)->visible = false;
    }
}

void RosterView::update(){
    
}

void RosterView::start(){
    // reset all values here
}

void RosterView::stop(){
    
}

void RosterView::draw(){
    if( !bVisible ){
        return;
    }
    
    if( mBg ){
        gl::color(1,1,1);
        gl::draw( mBg );
    }

    const bool isHandicapped = Model::instance().getIsHandicapped();
    for(int i=0; i < Model::instance().getNumRacers(); i++){
        mPlayerNames[i]->draw();

        
//        if( mPlayerNames[i]->isEnabled() ){
//            gl::color( Color::hex(0x717174) );
//            gl::drawSolidRect( mCancelRects[i] );
//            gl::color(1,1,1,1);
//            gl::draw( mCancelBtn, mCancelRects[i].getUpperLeft() + vec2(14, 12) );
//        }
        
        vec2 p = mPlayerNames[i]->getBounds().getUpperLeft() + vec2(-102, 73);
        if( !mPlayerNames[i]->isActive() ){
            gl::color( mPlayerNames[i]->mColorStroke );
            gl::drawSolidCircle(p, 62);
            gl::color( Color::hex(0x16151b) );
        }else {
            gl::lineWidth(4.0);
            gl::color( mPlayerNames[i]->mColorStroke );
            gl::drawStrokedCircle(p, 62);
            gl::lineWidth(1.0);
        }
                
        tFont->drawString( to_string(i+1), p + vec2(-19, 20) );
        
        if (isHandicapped) {
            mHandicaps[i] ->draw();
        }
    }
    
//    gui->draw();
}
