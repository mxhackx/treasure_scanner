#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cmath>

using namespace std;

class MSprite {
public:
    sf::Sprite sp;
    sf::Texture tx;
    explicit MSprite(string name);
    MSprite(const MSprite& other);
    MSprite& operator=(const MSprite& other);
};

MSprite::MSprite(string name) {
    if (!tx.loadFromFile(name)){
        cout << "Error while loading texture : " << name << endl;
        exit(84);
    }
    sp.setTexture(tx);
    sp.setScale(sf::Vector2f(0.3, 0.3));
}

MSprite::MSprite(const MSprite& other) : tx(other.tx) {
    sp = other.sp;
    sp.setTexture(tx);
}

MSprite& MSprite::operator=(const MSprite& other) {
    if (this != &other) {
        tx = other.tx;
        sp = other.sp;
        sp.setTexture(tx);
    }
    return *this;
}

class Grid {
public:
    enum Tile {
        Gem,
        Cross,
        Ring,
        Treasure,
    };

    // ================= CONSTANTES =================
    static constexpr float GRID_OFFSET_X = 510.f;
    static constexpr float GRID_OFFSET_Y = 20.f;

    static constexpr float CELL_SIZE_SMALL = 30.f;
    static constexpr float REVEAL_STEP_MS = 20.f;

    static constexpr float TREASURE_SCALE_SMALL = 0.3f;
    static constexpr float TREASURE_SCALE_BIG = 13.f;

    static constexpr float ZOOM_TIME = 3000.f;

    static constexpr float SHRINK_OFFSET_X = 490.f;
    static constexpr float ANIM_DIV = 3.f;
    static constexpr float TIME_SCALE = 100.f;

    static constexpr float FINAL_UI_X = 1050.f;
    static constexpr float FINAL_UI_Y1 = 400.f;
    static constexpr float FINAL_UI_Y2 = 480.f;
    static constexpr int CHARACTER_SIZE = 80;
    static constexpr float TREASURE_BIG_SIZE = 920.f;
    // ================= VARIABLES =================
    int height;
    int width;
    int size;
    int count;
    float time;
    bool find;
    int x;
    int y;
    int a;
    int b;
    int state;

    sf::Text t_count;
    sf::Text t_time;
    sf::Font font;

    vector<vector<Tile>> map;
    vector<vector<bool>> revealed;
    vector<MSprite> sprites;

    sf::RectangleShape r;
    sf::Clock clock;

    Grid(int h, int w, int s = CELL_SIZE_SMALL);
    void load_sprites(void);
    void gen_map();
    void draw(sf::RenderWindow& window);
    void draw_treasure(sf::RenderWindow& window);
};

// ================= CONSTRUCTOR =================
Grid::Grid(int h, int w, int s)
    : height(h),
      width(w),
      size(s),
      count(0),
      time(0.f),
      find(false),
      x(0),
      y(0),
      a(0),
      b(0),
      state(0)
{
    r.setSize(sf::Vector2f(size, size));
    r.setFillColor(sf::Color::Green);
    r.setOutlineThickness(size / 10.f);

    gen_map();
    load_sprites();

    if (!font.loadFromFile("./assets/arial.ttf")){
        cout << "Error while loading font" << endl;
        exit(84);
    }
    t_count.setFont(font);
    t_time.setFont(font);
    t_count.setFillColor(sf::Color::Blue);
    t_time.setFillColor(sf::Color::Blue);
    t_count.setCharacterSize(CHARACTER_SIZE);
    t_time.setCharacterSize(CHARACTER_SIZE);
    t_count.setString("");
    t_time.setString("");
}

// ================= MAP =================
void Grid::gen_map()
{
    map.resize(height, vector<Grid::Tile>(width));
    revealed.resize(height, vector<bool>(width));

    x = rand() % width;
    y = rand() % height;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            revealed[i][j] = false;

            if (i == y && j == x) {
                map[i][j] = Grid::Treasure;
                continue;
            }

            map[i][j] = static_cast<Tile>(rand() % Grid::Treasure);
        }
    }

    a = x * size;
    b = y * size;
}

// ================= SPRITES =================
void Grid::load_sprites()
{
    sprites.emplace_back("./assets/gem.png");
    sprites.emplace_back("./assets/cross.png");
    sprites.emplace_back("./assets/ring.png");
    sprites.emplace_back("./assets/treasure.png");
}

// ================= DRAW =================
void Grid::draw(sf::RenderWindow& window)
{
    if (state == 0) {
        count = 0;

        sprites[map[y][x]].sp.setScale(sf::Vector2f(TREASURE_SCALE_SMALL, TREASURE_SCALE_SMALL));
        r.setSize(sf::Vector2f(CELL_SIZE_SMALL, CELL_SIZE_SMALL));

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {

                r.setPosition(
                    GRID_OFFSET_X + j * size,
                    GRID_OFFSET_Y + i * size
                );

                window.draw(r);

                revealed[i][j] =
                    (revealed[i][j] == true)
                        ? true
                        : (clock.getElapsedTime().asMilliseconds() >= ((i * width + j) * REVEAL_STEP_MS))
                          && find == false;

                if (revealed[i][j] == true && map[i][j] == Treasure && find == false) {
                    find = true;
                    time = (float)clock.getElapsedTime().asMilliseconds() / 1000.f;
                }

                if (revealed[i][j] == true) {
                    count++;

                    sf::Sprite sprite = sprites[map[i][j]].sp;
                    sprite.setPosition(
                        GRID_OFFSET_X + j * size,
                        GRID_OFFSET_Y + i * size
                    );

                    window.draw(sprite);
                }
            }
        }
    }

    if (find == true)
        draw_treasure(window);
}
template <typename T>
string normalize_str(T a, int c)
{
    string n;
    string r = to_string(a);
    int s = r.size();

    if (s >= c)
        return r.substr(0, c);
    return string(c - s, '0') + r;
}
template <typename T>
string normalize_time_str(T s, int c, int t, int time = 30, string str = ": ", string end=" seconds")
{
    string n = normalize_str(s, c);
    int size = static_cast<int>(static_cast<float>(t * n.size()) / static_cast<float>(time));

    size = (size >= n.size()) ? n.size() : size;
    return str + n.substr(0, size) + end;
}

// ================= TREASURE ANIMATION =================
void Grid::draw_treasure(sf::RenderWindow& window)
{
    float temp = ZOOM_TIME;
    string s_count;
    string s_time;
    float c = a / temp;
    float d = b / temp;

    float t = ((float)clock.getElapsedTime().asMilliseconds() - time * 1000.f);

    float nx = GRID_OFFSET_X + a - c * t;
    float ny = GRID_OFFSET_Y + b - d * t;

    float scale = 12.7f / temp;
    float s = 900.f / temp;

    sf::Vector2f ts = r.getSize();

    if (nx >= GRID_OFFSET_X && state == 0) {
        sprites[map[y][x]].sp.setPosition(nx, ny);
        sprites[map[y][x]].sp.setScale(sf::Vector2f(TREASURE_SCALE_SMALL + scale * t, TREASURE_SCALE_SMALL + scale * t));

        r.setSize(sf::Vector2f(ts.x + s * t, ts.y + s * t));
        r.setPosition(nx, ny);
    }
    if (t >= temp && state == 0) {
        state = 1;
        sprites[map[y][x]].sp.setScale(sf::Vector2f(TREASURE_SCALE_BIG, TREASURE_SCALE_BIG));
        r.setSize(sf::Vector2f(TREASURE_BIG_SIZE, TREASURE_BIG_SIZE));
        r.setPosition(GRID_OFFSET_X, GRID_OFFSET_Y);
    }

    c = SHRINK_OFFSET_X / temp;

    t = (float)clock.getElapsedTime().asMilliseconds() - temp - time * 1000.f;

    nx = c * t;
    ny = 10;

    if (state == 1 && c * t <= SHRINK_OFFSET_X) {
        r.setPosition(GRID_OFFSET_X - nx, ny);
        sprites[map[y][x]].sp.setPosition(GRID_OFFSET_X - nx, ny);
    }

    if (state == 1 && c * t >= SHRINK_OFFSET_X) {
        state = 2;

        t_count.setPosition(sf::Vector2f(FINAL_UI_X, FINAL_UI_Y1));
        t_time.setPosition(sf::Vector2f(FINAL_UI_X, FINAL_UI_Y2));
    }

    t = (float)clock.getElapsedTime().asMilliseconds() - temp * 2.f - time * 1000.f;

    if (state == 2) {
        s_count = normalize_time_str(count, 3, static_cast<int>(t / 100.f), 30, "Count: ", " cases");
        s_time = normalize_time_str(time, 5, static_cast<int>(t / 100.f), 30, "Time: ", " seconds");
        t_count.setString(s_count);
        t_time.setString(s_time);

        window.draw(t_count);
        window.draw(t_time);
    }

    window.draw(r);
    window.draw(sprites[map[y][x]].sp);
}

// ================= MAIN =================
int main(int ac, char **av)
{
    int height = 30;
    int width = 30;
    int size = 30;
    if (ac == 4){
        if (stoi(av[1]) > 0)
            height = stoi(av[1]);
        if (stoi(av[2]) > 0)
            width = stoi(av[2]);
        if (stoi(av[3]) > 20)
            size = stoi(av[3]);
    }
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Treasure Scan");

    srand((unsigned int)time(0));

    Grid grid(height, width, size);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        grid.draw(window);
        window.display();
    }

    cout << grid.count << "    " << grid.time;
    return 0;
}
