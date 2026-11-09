#include "Game.h"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
int passed = 0, failed = 0;
void Check(bool ok, const std::string &label)
{
    std::cout << (ok ? "PASS " : "FAIL ") << label << '\n';
    if (ok)
        ++passed;
    else
        ++failed;
}
std::string Contents(const std::string &path)
{
    std::ifstream in(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(in), {}};
}
void Simulate(Game &g, float maxSeconds)
{
    for (int i = 0; i < static_cast<int>(maxSeconds * 60) && g.State() == GameState::ACTIVA; ++i)
        g.Update(1.0f / 60);
}
void LogicTests()
{
    std::filesystem::create_directories("tests/tmp");
    Enemy walker(1, {0, 0}, 10, 10);
    std::vector<Vector2> route{{0, 0}, {10, 0}, {10, 10}, {20, 10}};
    walker.Update(1.5f, route);
    Check(DistanceSquared(walker.Position(), {10, 5}) < 0.001f,
          "RF-01: avance y distancia sobrante en esquina");
    walker.Update(10, route);
    Check(walker.State() == EnemyState::Escaped &&
              DistanceSquared(walker.Position(), route.back()) == 0,
          "RF-01/RF-11: final exacto de ruta");
    auto pos = walker.Position();
    walker.Update(10, route);
    walker.TakeDamage(100);
    Check(walker.State() == EnemyState::Escaped && DistanceSquared(pos, walker.Position()) == 0,
          "RF-11: escape irreversible, sin eliminacion posterior");
    WaveManager waves;
    std::vector<Enemy> es;
    int starts = 0;
    for (int wave = 1; wave <= 8; ++wave)
    {
        int count = 0;
        for (int step = 0; step < 2000 && (waves.Current() < wave || waves.Remaining() > 0); ++step)
        {
            if (waves.Update(0.05f, es.empty(), es, {0, 0}))
                ++starts;
            count += static_cast<int>(es.size());
            es.clear();
        }
        Check(waves.Current() == wave && count == waves.CountFor(wave),
              "RF-02: conteo de oleada " + std::to_string(wave));
    }
    Check(starts == 8 && waves.Finished(true), "RF-02/RF-12: ocho oleadas y cierre");
    Check(waves.CountFor(8) > waves.CountFor(1) && waves.LifeFor(8) > waves.LifeFor(1) &&
              waves.SpeedFor(8) > waves.SpeedFor(1),
          "RF-13: incremento de cantidad, vida y velocidad");
    Game game({}, "tests/tmp/build.csv");
    Check(!game.TryBuild({144, 356}) && !game.SelectTower(TowerType::Basic),
          "MENU bloquea construccion");
    game.Start();
    Check(game.SelectTower(TowerType::Basic) && game.TryBuild({144, 356}) &&
              game.Towers()[0].Type() == TowerType::Basic,
          "RF-03: torre basica");
    Check(game.Coins() == 130, "RF-05: descuento unico de 50");
    Check(!game.TryBuild({144, 356}) && game.Coins() == 130 && game.Towers().size() == 1,
          "RF-04: ocupacion sin descuento");
    Check(!game.TryBuild({240, 252}) && !game.TryBuild({-10, 100}) && !game.TryBuild({940, 300}) &&
              game.Coins() == 130,
          "RF-04: camino, exterior y panel prohibidos");
    game.SelectTower(TowerType::LongRange);
    Check(game.TryBuild({336, 320}) && game.Coins() == 40 &&
              game.Towers().back().Type() == TowerType::LongRange,
          "RF-03/RF-05: torre larga y descuento de 90");
    Check(!game.TryBuild({576, 320}) && game.Coins() == 40 && game.Towers().size() == 2,
          "RF-04: saldo insuficiente sin descuento");
    // Verificar que ninguna casilla permitida invade los segmentos del camino.
    bool safe = true;
    auto &map = game.Map();
    for (auto p : map.BuildSlots())
        for (size_t i = 1; i < map.Waypoints().size(); ++i)
        {
            auto a = map.Waypoints()[i - 1], b = map.Waypoints()[i];
            float length = DistanceSquared(a, b);
            float t = std::clamp(((p.x - a.x) * (b.x - a.x) + (p.y - a.y) * (b.y - a.y)) / length,
                                 0.0f, 1.0f);
            if (DistanceSquared(p, {a.x + t * (b.x - a.x), a.y + t * (b.y - a.y)}) < 54 * 54)
                safe = false;
        }
    Check(safe, "RF-04: todas las casillas quedan fuera del camino");
    Tower tower(TowerType::Basic, {0, 0}, 0);
    std::vector<Enemy> enemies;
    enemies.emplace_back(10, Vector2{121, 0}, 30, 0.0f);
    Check(tower.FindTarget(enemies) == -1, "RF-06: excluir fuera de alcance");
    enemies.emplace_back(11, Vector2{120, 0}, 30, 0.0f);
    Check(tower.FindTarget(enemies) == 11, "RF-06: incluir borde exacto");
    enemies.emplace_back(12, Vector2{40, 0}, 30, 0.0f);
    enemies.emplace_back(13, Vector2{20, 0}, 30, 0.0f);
    Check(tower.FindTarget(enemies) == 13, "RF-07: objetivo valido mas cercano");
    enemies.back().TakeDamage(99);
    Check(tower.FindTarget(enemies) == 12, "RF-07/RF-09: ignorar eliminados");
    std::vector<Projectile> shots;
    tower.Update(0, enemies, shots);
    tower.Update(0.4f, enemies, shots);
    Check(shots.size() == 1, "RF-07: no disparar antes del intervalo");
    tower.Update(0.4f, enemies, shots);
    Check(shots.size() == 2, "RF-07: disparar al cumplir intervalo");
    shots[0].Update(1, enemies);
    Check(enemies[2].Life() == 18, "RF-08: impacto aplica 12 de dano");
    shots[0].Update(1, enemies);
    Check(enemies[2].Life() == 18, "RF-08: proyectil impacta una sola vez");
    Projectile stale({0, 0}, 999, 100);
    stale.Update(1, enemies);
    Check(!stale.Active(), "RF-09: retirar proyectil sin objetivo");
    Projectile overkill({0, 0}, 12, 100);
    overkill.Update(1, enemies);
    shots[1].Update(1, enemies);
    Check(enemies[2].Life() == 0 && !enemies[2].Active() && !shots[1].Active(),
          "RF-08/RF-09: vida cero y ataque concurrente seguro");
    GameConfig easy;
    easy.firstWaveCount = 1;
    easy.countGrowth = 0;
    easy.enemyLife = 1;
    easy.lifeGrowth = 0;
    easy.enemySpeed = 10;
    easy.speedGrowth = 0;
    easy.waveDelay = 0;
    Game victory(easy, "tests/tmp/victory.csv");
    victory.Start();
    victory.SelectTower(TowerType::LongRange);
    victory.TryBuild({144, 356});
    victory.TryBuild({336, 320});
    Simulate(victory, 150);
    Check(victory.State() == GameState::VICTORIA && victory.Waves().Current() == 8,
          "RF-12: victoria real al terminar ocho oleadas");
    Check(victory.Kills() == 8 && victory.Coins() == 80 && victory.Enemies().empty(),
          "RF-09/RF-10: retiro y recompensa unica bajo fuego de varias torres");
    float stopped = victory.ActiveTime();
    victory.Update(100);
    Check(victory.ActiveTime() == stopped && victory.Coins() == 80,
          "Resultado final no sigue simulando ni recompensando");
    std::string oldId = victory.MatchId();
    victory.Restart();
    Check(victory.State() == GameState::ACTIVA && victory.Coins() == 180 && victory.Life() == 20 &&
              victory.Kills() == 0 && victory.ActiveTime() == 0 && victory.Towers().empty() &&
              victory.Projectiles().empty() && victory.Waves().Current() == 0 &&
              victory.MatchId() != oldId,
          "RF-15: reinicio completo tras victoria");
    GameConfig fast;
    fast.enemySpeed = 10000;
    fast.waveDelay = 0;
    fast.spawnInterval = 0.01f;
    Game defeat(fast, "tests/tmp/defeat.csv");
    defeat.Start();
    Simulate(defeat, 10);
    Check(defeat.State() == GameState::DERROTA && defeat.Life() == 0 && defeat.Kills() == 0 &&
              defeat.Coins() == 180,
          "RF-11: dano unico, sin recompensa por escape, derrota y vida no negativa");
    oldId = defeat.MatchId();
    defeat.Restart();
    Check(defeat.Life() == 20 && defeat.State() == GameState::ACTIVA && defeat.MatchId() != oldId,
          "RF-15: reinicio tras derrota");
    GameConfig pauseConfig;
    pauseConfig.waveDelay = 0;
    Game paused(pauseConfig, "tests/tmp/pause.csv");
    paused.Start();
    paused.SelectTower(TowerType::LongRange);
    paused.TryBuild({144, 356});
    for (int i = 0; i < 400; ++i)
        paused.Update(1.0f / 60);
    // Buscar un cuadro que tenga simultáneamente enemigo, torre y proyectil.
    for (int i = 0; i < 1000 && paused.Projectiles().empty(); ++i)
        paused.Update(1.0f / 60);
    Check(!paused.Enemies().empty() && !paused.Projectiles().empty(),
          "Precondicion RF-14: combate en curso");
    float active = paused.ActiveTime(), timer = paused.Waves().Timer(),
          cooldown = paused.Towers()[0].Cooldown();
    auto before = paused.Enemies();
    auto beforeShots = paused.Projectiles();
    int coins = paused.Coins(), life = paused.Life(), wave = paused.Waves().Current();
    paused.TogglePause();
    for (int i = 0; i < 600; ++i)
        paused.Update(1.0f / 60);
    bool frozen = paused.State() == GameState::PAUSADA && active == paused.ActiveTime() &&
                  timer == paused.Waves().Timer() && cooldown == paused.Towers()[0].Cooldown() &&
                  coins == paused.Coins() && life == paused.Life() &&
                  wave == paused.Waves().Current() && before.size() == paused.Enemies().size() &&
                  beforeShots.size() == paused.Projectiles().size();
    for (size_t i = 0; i < before.size(); ++i)
        frozen &= DistanceSquared(before[i].Position(), paused.Enemies()[i].Position()) == 0 &&
                  before[i].Life() == paused.Enemies()[i].Life();
    for (size_t i = 0; i < beforeShots.size(); ++i)
        frozen &=
            DistanceSquared(beforeShots[i].Position(), paused.Projectiles()[i].Position()) == 0;
    Check(frozen && !paused.TryBuild({576, 320}) && !paused.SelectTower(TowerType::Basic),
          "RF-14: pausa conserva entidades, vida, economia y todos los temporizadores");
    paused.TogglePause();
    paused.Update(0.1f);
    Check(paused.State() == GameState::ACTIVA && paused.ActiveTime() > active,
          "RF-14: reanudacion");
    const auto csv = Contents("tests/tmp/pause.csv");
    Check(csv.find("PAUSA") != std::string::npos && csv.find("REANUDACION") != std::string::npos,
          "CSV: eventos de pausa y reanudacion");
    Check(CsvLogger::Escape("a,b\"c\nd") == "\"a,b\"\"c\nd\"",
          "CSV: escape de coma, comillas y salto de linea");
    std::string file = "tests/tmp/format-" + CsvLogger::NewMatchId() + ".csv";
    CsvLogger logger(file);
    LogEvent event{"test", 1.5f, "PAUSA", 1, 20, 180, 0, "", "coma, comillas\" y\nsalto"};
    Check(logger.Write(event) && logger.Write(event), "CSV: escritura y acumulacion");
    const auto text = Contents(file);
    auto header = std::string(CsvLogger::Header());
    Check(text.starts_with(header) && text.find(header, header.size()) == std::string::npos,
          "CSV: encabezado exacto sin duplicar");
    {
        std::ofstream bad("tests/tmp/bad.csv", std::ios::binary);
        bad << "incorrecto\n";
    }
    CsvLogger bad("tests/tmp/bad.csv");
    Check(!bad.Write(event) && !bad.Error().empty() &&
              Contents("tests/tmp/bad.csv") == "incorrecto\n",
          "CSV: rechazar encabezado incompatible sin sobrescribir");
    CsvLogger directory("tests/tmp");
    Check(!directory.Write(event) && !directory.Error().empty(),
          "CSV: error de escritura controlado");
    Check(CsvLogger::Timestamp().ends_with("Z"), "CSV: fecha ISO 8601 en UTC");
    Game campaign({}, "tests/tmp/default-campaign.csv");
    campaign.Start();
    campaign.SelectTower(TowerType::LongRange);
    std::vector<Vector2> order{{336, 320}, {576, 320}, {144, 356}, {384, 344}, {432, 160},
                               {576, 500}, {96, 452},  {624, 500}, {768, 476}, {720, 176},
                               {336, 548}, {384, 548}, {744, 548}};
    size_t next = 0;
    for (int tick = 0; tick < 900 * 60 && campaign.State() == GameState::ACTIVA; ++tick)
    {
        if (next < order.size() && campaign.Coins() >= 90 && campaign.TryBuild(order[next]))
            ++next;
        campaign.Update(1.0f / 60);
    }
    std::cout << "DEFAULT_CAMPAIGN state=" << static_cast<int>(campaign.State())
              << " life=" << campaign.Life() << " kills=" << campaign.Kills()
              << " time=" << campaign.ActiveTime() << " towers=" << campaign.Towers().size()
              << '\n';
    Check(campaign.State() == GameState::VICTORIA,
          "Integracion: ocho oleadas con parametros reales y construccion estrategica");
    std::cout << "RESULT " << passed << " passed, " << failed << " failed\n";
}
void Screenshots()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(1280, 720, "TowerDefense2D - evidencia automatizada");
    SetTargetFPS(60);
    auto capture = [](Game &g, const char *file, Vector2 pointer = Vector2{-1, -1}) {
        BeginDrawing();
        g.Draw(pointer);
        EndDrawing();
        TakeScreenshot(file);
    };
    Game g({}, "tests/tmp/graphics.csv");
    g.LoadAssets();
    capture(g, "docs/evidence/01-menu.png");
    g.Start();
    g.SelectTower(TowerType::Basic);
    g.TryBuild({336, 320});
    g.SelectTower(TowerType::LongRange);
    g.TryBuild({576, 320});
    for (int i = 0; i < 900; ++i)
        g.Update(1.0f / 60);
    capture(g, "docs/evidence/02-partida.png");
    g.TogglePause();
    capture(g, "docs/evidence/03-pausa.png");
    g.TogglePause();
    GameConfig win;
    win.totalWaves = 1;
    win.firstWaveCount = 1;
    win.enemyLife = 1;
    win.waveDelay = 0;
    {
        Game v(win, "tests/tmp/graphics-win.csv");
        v.LoadAssets();
        v.Start();
        v.SelectTower(TowerType::LongRange);
        v.TryBuild({144, 356});
        Simulate(v, 100);
        Check(v.State() == GameState::VICTORIA, "Grafica: victoria obtenida por simulacion");
        capture(v, "docs/evidence/04-victoria.png");
    }
    GameConfig lose;
    lose.enemySpeed = 10000;
    lose.waveDelay = 0;
    lose.spawnInterval = 0.01f;
    {
        Game d(lose, "tests/tmp/graphics-lose.csv");
        d.LoadAssets();
        d.Start();
        Simulate(d, 10);
        Check(d.State() == GameState::DERROTA, "Grafica: derrota obtenida por simulacion");
        capture(d, "docs/evidence/05-derrota.png");
    }
    {
        Game p({}, "tests/tmp/graphics-preview.csv");
        p.LoadAssets();
        p.Start();
        p.SelectTower(TowerType::Basic);
        capture(p, "docs/evidence/07-colocacion-valida.png", {576, 500});
        capture(p, "docs/evidence/08-colocacion-invalida.png", {480, 252});
    }
    // g libera texturas antes de cerrar el contexto OpenGL.
}
void Benchmark()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(1280, 720, "TowerDefense2D - rendimiento 30+ entidades");
    SetTargetFPS(60);
    {
        GameConfig c;
        c.firstWaveCount = 70;
        c.countGrowth = 0;
        c.enemyLife = 100000;
        c.enemySpeed = 8;
        c.spawnInterval = 0.03f;
        c.waveDelay = 0;
        c.initialCoins = 5000;
        Game g(c, "tests/tmp/benchmark.csv");
        g.LoadAssets();
        g.Start();
        g.SelectTower(TowerType::LongRange);
        for (auto p : g.Map().BuildSlots())
            g.TryBuild(p);
        for (int i = 0; i < 300; ++i)
            g.Update(1.0f / 60);
        std::ofstream raw("docs/evidence/frame-times.csv");
        raw << "frame,frame_ms,entities,update_draw_cpu_ms\n";
        std::vector<double> ms;
        size_t minEntities = 99999;
        double cpuMax = 0;
        for (int i = 0; i < 1920; ++i)
        {
            double begin = GetTime();
            g.Update(1.0f / 60);
            BeginDrawing();
            g.Draw();
            double cpu = (GetTime() - begin) * 1000;
            EndDrawing();
            size_t entities = g.Enemies().size() + g.Towers().size() + g.Projectiles().size();
            if (i >= 120)
            {
                double elapsed = GetFrameTime() * 1000;
                ms.push_back(elapsed);
                minEntities = std::min(minEntities, entities);
                cpuMax = std::max(cpuMax, cpu);
                raw << i << ',' << elapsed << ',' << entities << ',' << cpu << '\n';
            }
            if (i == 240)
                TakeScreenshot("docs/evidence/06-rendimiento.png");
        }
        std::sort(ms.begin(), ms.end());
        double avg = std::accumulate(ms.begin(), ms.end(), 0.0) / ms.size();
        std::ofstream result("docs/evidence/benchmark.json");
        result << "{\n  \"frames\": " << ms.size() << ", \"min_entities\": " << minEntities
               << ",\n  \"mean_frame_ms\": " << avg << ", \"mean_fps\": " << 1000 / avg
               << ",\n  \"p95_frame_ms\": " << ms[static_cast<size_t>(ms.size() * 0.95)]
               << ", \"p99_frame_ms\": " << ms[static_cast<size_t>(ms.size() * 0.99)]
               << ",\n  \"max_frame_ms\": " << ms.back()
               << ", \"max_update_draw_cpu_ms\": " << cpuMax << "\n}\n";
        std::cout << "BENCHMARK " << ms.size() << " frames, min entities " << minEntities
                  << ", mean FPS " << 1000 / avg << ", p99 ms "
                  << ms[static_cast<size_t>(ms.size() * 0.99)] << '\n';
    }
    CloseWindow();
}
} // namespace
int main(int argc, char **argv)
{
    if (argc > 1 && std::string(argv[1]) == "--screenshots")
    {
        Screenshots();
        CloseWindow();
    }
    else if (argc > 1 && std::string(argv[1]) == "--benchmark")
        Benchmark();
    else
        LogicTests();
    return failed ? 1 : 0;
}
