////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>

#include <vector>
#include <string>
using namespace std;
////////////////////////////////////////////////////////////
/// GUI State
const int FONT_SIZE = 24;
const int FONT_WIDTH = 14;
                        
struct State
{
    // General resources
    sf::RenderWindow window;
    const sf::Font font{"../resources/dejavu-sans-mono-font/DejavuSansMono-5m7L.ttf"};
    // Text buffer
    std::vector<std::string> log;
    sf::IntRect text_view;
    sf::Vector2i cursor_pos;
    State(unsigned w, unsigned h, std::string title)
    {
        window = sf::RenderWindow(sf::VideoMode({w, h}), title);
        // window.setVerticalSyncEnabled(true); // alternative to setFramerateLimit
        window.setFramerateLimit(60);
        log.resize(1, "");
        text_view = sf::IntRect({0,0},{static_cast<int>(w/FONT_WIDTH-2),static_cast<int>(h/FONT_SIZE-2)});
        cursor_pos = {0,0};
    }
    void adjustView() {
        auto size = window.getSize();
        text_view.size.x = size.x / FONT_WIDTH - 2;
        text_view.size.y = size.y / FONT_SIZE - 2;
        if (cursor_pos.x < text_view.position.x) {
            text_view.position.x = cursor_pos.x;
        } else if (cursor_pos.x >= text_view.position.x + text_view.size.x) {
            text_view.position.x = cursor_pos.x - text_view.size.x + 1;
        }
        if (cursor_pos.y < text_view.position.y) {
            text_view.position.y = cursor_pos.y;
        } else if (cursor_pos.y >= text_view.position.y + text_view.size.y) {
            text_view.position.y = cursor_pos.y - text_view.size.y + 1;
        }
    }
};
///
//////////////////////////////////////////////////////////// 

////////////////////////////////////////////////////////////
/// Callback functions
void handle(const sf::Event::Closed &, State &gs)
{
    gs.window.close();
}

void handle(const sf::Event::TextEntered &textEnter, State &gs)
{
    unsigned last = gs.log.size() - 1;
    // if (textEnter.unicode == 10 || textEnter.unicode == 13)
    if (textEnter.unicode == '\n' || textEnter.unicode == '\r') // enter
    {
        std::string rest = gs.log[gs.cursor_pos.y].substr(gs.cursor_pos.x);
        gs.log[gs.cursor_pos.y] = gs.log[gs.cursor_pos.y].substr(0, gs.cursor_pos.x);
        gs.cursor_pos.y++;
        gs.log.insert(gs.log.begin() + gs.cursor_pos.y, rest);
        gs.cursor_pos.x = 0;
    }
    else if (textEnter.unicode >= ' ' && textEnter.unicode <= '~') // printable char
    {
        gs.log[gs.cursor_pos.y].insert(gs.cursor_pos.x, 1, static_cast<char>(textEnter.unicode));
        gs.cursor_pos.x++;
    }
    else if (textEnter.unicode == 8) // backspace
    {
        if (gs.cursor_pos.x > 0) {
            gs.log[gs.cursor_pos.y].erase(gs.cursor_pos.x - 1, 1);
            gs.cursor_pos.x--;
        } else if (gs.cursor_pos.y > 0) {
            gs.cursor_pos.x = gs.log[gs.cursor_pos.y - 1].size();
            gs.log[gs.cursor_pos.y - 1] += gs.log[gs.cursor_pos.y];
            gs.log.erase(gs.log.begin() + gs.cursor_pos.y);
            gs.cursor_pos.y--;
        }
    }
    gs.adjustView();
}

void handle(const sf::Event::KeyPressed &keyPressed, State &gs)
{
    if (keyPressed.code == sf::Keyboard::Key::Up) {
        if (gs.cursor_pos.y > 0) {
            gs.cursor_pos.y--;
            if (gs.cursor_pos.x > static_cast<int>(gs.log[gs.cursor_pos.y].size())) {
                gs.cursor_pos.x = gs.log[gs.cursor_pos.y].size();
            }
        }
    } else if (keyPressed.code == sf::Keyboard::Key::Down) {
        if (gs.cursor_pos.y < static_cast<int>(gs.log.size()) - 1) {
            gs.cursor_pos.y++;
            if (gs.cursor_pos.x > static_cast<int>(gs.log[gs.cursor_pos.y].size())) {
                gs.cursor_pos.x = gs.log[gs.cursor_pos.y].size();
            }
        }
    } else if (keyPressed.code == sf::Keyboard::Key::Left) {
        if (gs.cursor_pos.x > 0) {
            gs.cursor_pos.x--;
        } else if (gs.cursor_pos.y > 0) {
            gs.cursor_pos.y--;
            gs.cursor_pos.x = gs.log[gs.cursor_pos.y].size();
        }
    } else if (keyPressed.code == sf::Keyboard::Key::Right) {
        if (gs.cursor_pos.x < static_cast<int>(gs.log[gs.cursor_pos.y].size())) {
            gs.cursor_pos.x++;
        } else if (gs.cursor_pos.y < static_cast<int>(gs.log.size()) - 1) {
            gs.cursor_pos.y++;
            gs.cursor_pos.x = 0;
        }
    }
    gs.adjustView();
}

// void handle(const sf::Event::MouseMoved &mouseMoved, State &gs)
// {
// }

// void handle(const sf::Event::MouseButtonPressed &, State &gs)
// {
// }

void handle(const sf::Event::Resized &resized, State &gs)
{
    sf::FloatRect visibleArea({0.f, 0.f}, sf::Vector2f(resized.size));
    gs.window.setView(sf::View(visibleArea));
    gs.adjustView();
}

template <typename T>
void handle(const T &, State &gs)
{
    // All unhandled events will end up here
}
///
////////////////////////////////////////////////////////////
void doGraphics(State &gs, sf::Text &logText){
    // Show log
    unsigned lines_to_print = std::min(static_cast<unsigned>(gs.log.size())-gs.text_view.position.y, 
    static_cast<unsigned>(gs.text_view.size.y));
    gs.window.clear();
    for (std::size_t i = 0; i < lines_to_print; ++i)
    {
        logText.setPosition({FONT_WIDTH,static_cast<float>(i * FONT_SIZE)+FONT_SIZE});
        if(gs.text_view.position.y<gs.log[i+gs.text_view.position.y].size()){
            logText.setString(gs.log[i+gs.text_view.position.y].substr((size_t)gs.text_view.position.x,size_t(gs.text_view.size.x)));
            gs.window.draw(logText);
        }
    }
    for(std::size_t i = 0; i < lines_to_print; ++i){
        if(gs.text_view.position.y+i == gs.cursor_pos.y){
            sf::Text cursorText{gs.font, "_", FONT_SIZE};
            cursorText.setFillColor(sf::Color::Green);
            int window_x = (gs.cursor_pos.x - gs.text_view.position.x + 1) * FONT_WIDTH;
            int window_y = (gs.cursor_pos.y - gs.text_view.position.y + 1) * FONT_SIZE + 3;
            cursorText.setPosition({static_cast<float>(window_x), static_cast<float>(window_y)});
            gs.window.draw(cursorText);
        }
    }
    gs.window.display();
    // Draw cursor
    // sf::Text cursorText{gs.font, "_", FONT_SIZE};
    // cursorText.setFillColor(sf::Color::Green);
    
    // // Convert table coordinates to window coordinates
    // // Add 50 pixel offset to match text position (same margin as text drawing)
    // int window_x = (gs.cursor_pos.x - gs.text_view.position.x) * FONT_WIDTH;
    // int window_y = (gs.cursor_pos.y - gs.text_view.position.y) * FONT_SIZE + 3;
    
    // cursorText.setPosition({static_cast<float>(window_x), static_cast<float>(window_y)});
    // gs.window.draw(cursorText);
    
    // gs.window.display();
}
int main()
{
    State gs(800, 600, "Text editor");
    sf::Text logText{gs.font, "", FONT_SIZE};
    while (gs.window.isOpen()) // main loop
    {
        // event loop and handler through callbacks
        gs.window.handleEvents([&](const auto &event)
                               { handle(event, gs); });
        doGraphics(gs,logText);
        
    }
}
