#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <fstream>
//#include <SFML/Network.hpp>

using namespace sf;

std::vector<std::string> list;
std::set<std::pair<std::string, std::string> > edges;
std::vector<std::vector<std::string> > depth; 

struct node {
	Text t;
	int x, y;
	std::string s;
	node(Text t, int x, int y, std::string s) : t(t), x(x), y(y), s(s) {}
};

void DrawGraph(RenderWindow& window) {
	Text prog;
	Font font;
	font.loadFromFile("font.ttf");
	prog.setFont(font);
	prog.setCharacterSize(50);
	prog.setFillColor(Color::White);

	int pos;
	int delta;
	int mid = 670;
	
	std::vector<int> sum(depth.size());
	for (int i = 0; i < depth.size(); i++) {
		for (int j = 0; j < depth[i].size(); j++) {
			sum[i] += (int)depth[i][j].size();
		}
	}

	std::vector<node> data;
	pos = 5;
	for (int i = 0; i < depth.size(); i++) {
		delta = mid - sum[i] * 10;
		for (int j = 0; j < depth[i].size(); j++) {
			prog.setPosition(delta, pos);
			prog.setString(depth[i][j]);
			data.push_back(node(prog, delta, pos, depth[i][j]));
			delta += depth[i][j].size() * 23;
		}
		pos += 100;
	}

	int deltax = 0;
	int deltay = 0;
	while (window.isOpen()) {
		window.clear();
		
		Event key;
		while (window.pollEvent(key)) {
			if (key.type == Event::Closed) {
				window.close();
			}
			else if (Keyboard::isKeyPressed(Keyboard::Down)) {
				deltay += 50;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Up)) {
				deltay -= 50;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Left)) {
				deltax -= 50;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Right)) {
				deltax += 50;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				return;
			}
		}

		for (int i = 0; i < data.size(); i++) {
			for (int j = 0; j < data.size(); j++) {
				if (data[i].y >= data[j].y) {
					continue;
				}
				if (edges.find(make_pair(data[i].s, data[j].s)) != edges.end()) {
					int center1 = data[i].x + data[i].s.size() * 19 / 2;
					CircleShape point;
					point.setFillColor(Color(34, 59, 250));
					point.setRadius(1);
					
					int half = data[i].y + 50 + 30;
					for (int y = data[i].y + 50; y <= half; y++) {
						point.setPosition(center1 - deltax, y - deltay);
						window.draw(point);
					}

					int center2 = data[j].x + data[j].s.size() * 19 / 2;
					for (int y = data[j].y + 10; y >= half; y--) {
						point.setPosition(center2 - deltax, y - deltay);
						window.draw(point);
					}

					for (int x = std::min(center1, center2); x <= std::max(center1, center2); x++) {
						point.setPosition(x - deltax, half - deltay);
						window.draw(point);
					}
				}
			}
		}

		for (auto i : data) {
			auto tmp = i;
			tmp.x -= deltax;
			tmp.y -= deltay;
			tmp.t.setPosition(tmp.x, tmp.y);
			window.draw(tmp.t);
		}

		window.display();
	}
}

void work(RenderWindow& window) {
	Text prog;
	Font font;
	font.loadFromFile("font.ttf");
	prog.setFont(font);
	prog.setCharacterSize(100);
	prog.setFillColor(Color::White);

	Texture tx;
	tx.loadFromFile("fkn.png");
	Sprite bottom(tx);
	bottom.setPosition(1100, 310);

	int pos;
	int delta = 0;
	while (window.isOpen()) {
		bool f = false;
		bottom.setColor(Color::White);

		if (IntRect(1100, 310, 109, 109).contains(Mouse::getPosition(window))) { 
			f = true;
			bottom.setColor(Color::Yellow);
		}

		Event key;
		while (window.pollEvent(key)) {
			if (key.type == Event::Closed) {
				window.close();
			}
			else if (Keyboard::isKeyPressed(Keyboard::Down)) {
				delta += (5 + 100 * list.size() - delta > 700 ? 50 : 0);
			}
			else if (Keyboard::isKeyPressed(Keyboard::Up)) {
				delta -= delta == 0 ? 0 : 50;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				return;
			}
			else if (Mouse::isButtonPressed(Mouse::Left)) {
				DrawGraph(window);
			}
		}
		
		pos = 5;
		window.clear();

		for (auto s : list) {
			prog.setString(s);
			prog.setPosition(670 - 18 * s.size(), pos - delta);
			window.draw(prog);
			pos += 100;
		}
		window.draw(bottom);
		window.display();
	}
}

std::map<std::string, std::vector<std::string> > grph;
int cnt = 1;

std::string to_string(int num) {
	std::string res = "";
	while (num) {
		res = (char)((num % 10) + '0') + res;
		num /= 10;
	}
	return res;
}

void dfs(std::string v, std::map<std::string, bool>& used, int d) {
	used[v] = true;
	if (depth.size() <= d) {
		std::vector<std::string> tmp;
		tmp.push_back(v);
		depth.push_back(tmp);
	}
	else {
		depth[d].push_back(v);
	}
	for (auto u : grph[v]) {
		if (!used[u]) {
			dfs(u, used, d + 1);
		}
	}

	std::string ans = v;
	ans = to_string(cnt) + ". " + ans;
	cnt++;
	list.push_back(ans);
	return;
}

void read(std::string s) {
	std::ifstream cin(s);

	int n;
	cin >> n;
	for (int i = 0; i < n; i++) {
		std::string s1, s2;
		cin >> s1 >> s2;
		grph[s1].push_back(s2);
		edges.insert(std::make_pair(s1, s2));
		edges.insert(std::make_pair(s2, s1));
	}
}

/*void GetFile(std::string s) {
	Http http("http://10.21.139.171:8081");

	std::string tmp = "/";
	tmp += s;

	sf::Http::Request request;
	request.setMethod(sf::Http::Request::Get);
	request.setUri(tmp);
	
	auto pred = time(0);
	Http::Response response = http.sendRequest(request);
	while (response.getStatus() != Http::Response::Ok) {
		if (time(0) - pred < 1) {
			continue;
		}
		pred = time(0);
		response = http.sendRequest(request);
	}
	std::cout << response.getBody();
	//read(response.getBody());
}*/

int main(){

	read("vim.txt");
	read("gcc.txt");

	RenderWindow window;
	window.create(VideoMode(1366, 768), "inDependencies");
	window.setFramerateLimit(60);

	std::string str = "";
	Text prog;
	Font font;
	font.loadFromFile("font.ttf");
	prog.setFont(font);
	prog.setCharacterSize(100);
	prog.setFillColor(Color::White);
	prog.setPosition(670, 310);

	int textx = 670;
	int texty = 310;

	while(window.isOpen()){
		Event key;
		while (window.pollEvent(key)) {
			if (key.type == Event::Closed) {
				window.close();
			}
			else if (key.type == Event::TextEntered) {
				if (Keyboard::isKeyPressed(Keyboard::Return)) {
					//GetFile(str);
					std::map<std::string, bool> used;
					dfs(str, used, 0);
					work(window);
					str = "";
					depth.clear();
					list.clear();
					cnt = 1;
					textx = 670;
				}
				else if (Keyboard::isKeyPressed(Keyboard::BackSpace)) {
					if (str.size() > 0) {
						str = str.substr(0, str.size() - 1);
						textx += 18;
					}
				}
				else {
					str += static_cast<char>(key.text.unicode);
					textx -= 18;
				}
			}
		}

		prog.setString(str);
		prog.setPosition(textx, texty);

		window.clear();

		window.draw(prog);
		window.display();
		
	}

}