#ifndef BOT_HPP
#define BOT_HPP
struct Server;
struct Unit;
struct Area;
struct BotInformation;
void moveBot(Server& server,Unit& u, const Area& area, const std::vector<Unit>& units, BotInformation* yourInfo);

#endif
