#include "ParticleEditorGUI.h"
#include "SDLHelpers.h"
#include "Button.h"
#include "Slider.h"
#include "Window.h"
#include "ParticleEmitterFactory.h"

using namespace std;

#define COLOR_NORMAL Color(128, 128, 192, 255)
#define COLOR_HIGHLIGHTED Color(192, 192, 255, 255)
#define COLOR_PRESSED Color(96, 96, 160, 255)
#define COLOR_SLIDER_BG Color(48, 48, 96, 255) 
#define COLOR_PANEL_BG Color(0, 0, 32, 255)
#define COLOR_FONT Color(255, 255, 255, 255)

#define FPS_COUNTER_POS_X 685
#define FPS_COUNTER_POS_Y 0

#define GUI_WIDTH 200
#define MIN_DISTANCE_EMITTER_GUI 100 //so you can still see particles while interacting with GUI
#define TOP_WIDGET_POS_Y WIDGET_SPACING/2
#define WIDGET_SPACING 30
#define WIDGET_HEIGHT 14
#define FONT_SIZE 12

int ParticleEditorGUI::PlaceBelowPrevious(int startY) {
	static int offset = -WIDGET_SPACING;
	offset += WIDGET_SPACING;
	return startY + offset;
}

//Creates a button/slider using the standard measurements, colours and font.
void ParticleEditorGUI::CreateButton(const string &label, function<void()> *onClick) {
	_widgets.push_back(new Button(Rect(_posX+10, PlaceBelowPrevious(TOP_WIDGET_POS_Y), GUI_WIDTH-20, WIDGET_HEIGHT), 
		COLOR_NORMAL, COLOR_HIGHLIGHTED, COLOR_PRESSED, _font, COLOR_FONT, 
		label, onClick));
}
template<class T>
void ParticleEditorGUI::CreateSlider(const string &label, T &bindVariable, T min, T max) {
	auto getter = std::function<T()>([&bindVariable] () { return bindVariable; }); 
	auto setter = std::function<void(T)>([&bindVariable] (T val) { bindVariable = val; });
	CreateSlider<T>(label, getter, setter, min, max);
}
template<class T>
void ParticleEditorGUI::CreateSlider(const string &label, const function<T()> &getter, const function<void(T)> &setter, T min, T max) {
	_widgets.push_back(new Slider<T>(Rect(_posX+10, PlaceBelowPrevious(TOP_WIDGET_POS_Y), GUI_WIDTH-20, WIDGET_HEIGHT), 
		COLOR_NORMAL, COLOR_HIGHLIGHTED, COLOR_PRESSED, COLOR_SLIDER_BG, _font, COLOR_FONT, 
		label, getter, setter, min, max));
}

ParticleEditorGUI::ParticleEditorGUI(int posX, int posY, int screenHeight) {
	_emitter = ParticleEmitterFactory::createFireExhaustSlow(Vec2(0, 0));
	_font = Window::LoadFont("font/SourceSansPro-Regular.ttf", FONT_SIZE);
	_posX = posX;
	_posY = posY;

	_backgroundRect = Rect(posX, 0, 200, screenHeight);
	_backgroundColor = COLOR_PANEL_BG;

	CreateButton("Reset", new function<void()>([this] () { delete _emitter; _emitter = ParticleEmitterFactory::createFireExhaustSlow(Vec2(0,0)); }));

	CreateSlider<float>("Emit/sec", [this] () { return _emitter->EmitPerSecond; },
		[this] (float val) { _emitter->EmitPerSecond = _emitter->_originalEmitPerSecond = val; }, 1, 1000);
	CreateSlider<float>("Min Life (sec)", _emitter->MinTTL, 0, 10);
	CreateSlider<float>("Max Life (sec)", _emitter->MaxTTL, 0, 10);
	CreateSlider<float>("Min Speed", _emitter->MinSpeed, 0, 500);
	CreateSlider<float>("Max Speed", _emitter->MaxSpeed, 0, 500);
	CreateSlider<float>("Min Rotation Speed (deg/sec)", _emitter->MinAngularVelocity, 0, 720);
	CreateSlider<float>("Max Rotation Speed (deg/sec)", _emitter->MaxAngularVelocity, 0, 720);
	CreateSlider<float>("Min Scale", _emitter->MinScale, 0, 2);
	CreateSlider<float>("Max Scale", _emitter->MaxScale, 0, 2);
	CreateSlider<float>("Min Growth Rate", _emitter->MinScalingVelocity, -2, 2);
	CreateSlider<float>("Max Growth Rate", _emitter->MaxScalingVelocity, -2, 2);
	CreateSlider<double>("Wind", _emitter->ParticleAcceleration.X, -250, 250);
	CreateSlider<double>("Gravity", _emitter->ParticleAcceleration.Y, -250, 250);
	CreateSlider<float>("Spread Constraint (deg)", _emitter->SpreadConstraint, 0, 360);
	CreateSlider<float>("Emitter Angle (deg)", _emitter->EmitterAngle, 0, 360);
	CreateSlider<float>("Fade at Age", _emitter->FadeParticlesAtAge, 0, 1);
	CreateSlider<float>("Emitter Life (sec)", _emitter->EmitterTTL, -1, 10);
	
	CreateSlider<Uint8>("Red", [this] () { return _emitter->Red(); }, 
		[this] (Uint8 val) { _emitter->Red(val); }, 0, 255);
	CreateSlider<Uint8>("Green", [this] () { return _emitter->Green(); }, 
		[this] (Uint8 val) { _emitter->Green(val); }, 0, 255);
	CreateSlider<Uint8>("Blue", [this] () { return _emitter->Blue(); }, 
		[this] (Uint8 val) { _emitter->Blue(val); }, 0, 255);
	CreateSlider<Uint8>("Alpha", _emitter->StartAlpha, 0, 255);
	
	CreateButton("Blend Mode", new function<void()>(
		[this] () {_emitter->BlendMode((SDL_BlendMode)((_emitter->BlendMode()+1) % 4)); }));
}

ParticleEditorGUI::~ParticleEditorGUI() {
	for (auto widget : _widgets)
		delete widget;
	_widgets.clear();

	TTF_CloseFont(_font);
}

void ParticleEditorGUI::Update(double dt, int mouseX, int mouseY, bool pressed) {
	if (!_emitter->IsAlive()) {
		_emitter->EmitPerSecond = _emitter->_originalEmitPerSecond;
		_emitter->_timePassed = 0;
	}
	
	_emitter->Centre = Vec2(constrain(mouseX, 0, _posX - MIN_DISTANCE_EMITTER_GUI), mouseY);
	_emitter->Update(dt);

	for (auto widget : _widgets)
		widget->Update(mouseX, mouseY, pressed);
}

void ParticleEditorGUI::Draw(double msPerFrame) {
	//Draw emitter
	_emitter->Draw();

	//Draw framerate
	stringstream ss;
	ss << msPerFrame << "ms/Frame" << " = " << floor(1000.0/msPerFrame) << "fps";
	Window::Draw(ss.str(), _font, COLOR_FONT, FPS_COUNTER_POS_X, FPS_COUNTER_POS_Y);

	//Draw GUI background (panel)
	SDL_SetRenderDrawColor(Window::Renderer(), _backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a); 
	SDL_RenderFillRect(Window::Renderer(), &_backgroundRect);

	//Draw all widgets
	for (auto widget : _widgets)
		widget->Draw();
}