#ifndef BOT_HPP
#define BOT_HPP

struct Unit;
struct Area;
struct BotInformation;
void moveBot(Unit& u, const Area& area, const std::vector<Unit>& units, BotInformation* yourInfo);

#endif
