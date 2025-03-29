#include <iostream>
#include <vector>

uint64_t Part(int64_t val) {
  // for negative values take only lower 32 bits
  if (val < 0) val &= 0xFFFFFFFF;

  val = (val | (val << 16)) & 0x0000FFFF0000FFFF;
  val = (val | (val << 8)) & 0x00FF00FF00FF00FF;
  val = (val | (val << 4)) & 0x0F0F0F0F0F0F0F0F;
  val = (val | (val << 2)) & 0x3333333333333333;
  val = (val | (val << 1)) & 0x5555555555555555;
  return val;
}

uint64_t GetMortonCodeFromNds(int x, int y) {
  // Cut y to 31 bits before (as latitude occupy 31 bits only)
  return Part(static_cast<int64_t>(x)) | (Part(static_cast<int64_t>(y) & 0x7FFFFFFF) << 1);
}

uint32_t GetTileIdFromMorton(int level, uint64_t morton_code) {
  uint32_t packed_tile_id = morton_code >> (62 - 2 * level);
  uint32_t packed_level = 1 << (16 + level);
  return packed_tile_id | packed_level;
}

double GetMapResolution(int level) {
//   DCHECK_LE(level, 15);
//   DCHECK_GE(level, 0);
  return 180.0 / (1 << level);
}

int GetNdsCoordFromDeg(double deg_val) {
//   DCHECK(deg_val >= -180.0 && deg_val <= 180.0);
  return static_cast<int>((1 << 30) * deg_val / 90.0);
}

uint32_t GetTileIdFromNds(int level, int nds_x, int nds_y) {
  uint64_t morton_code = GetMortonCodeFromNds(nds_x, nds_y);
  return GetTileIdFromMorton(level, morton_code);
}

uint32_t GetTileId(int level, double lon, double lat) {
//   DCHECK(lon >= -180.0 && lon <= 180.0);
//   DCHECK(lat >= -90.0 && lat <= 90.0);
  int x = GetNdsCoordFromDeg(lon);
  int y = GetNdsCoordFromDeg(lat);
  return GetTileIdFromNds(level, x, y);
}

int GetTileWidth(int level) { return 1 << (31 - level); }

int GetPackedTileIdLevel(uint32_t tile_id) {
  int level = 0;
  uint32_t temp_tile_id = tile_id;
  while (temp_tile_id) {
    temp_tile_id >>= 1;
    ++level;
  }
  return level - 17;
}

int SignedBitRestore(int val) { return val & 0x40000000 ? val | 0x80000000 : val; }

int CompactBits(uint64_t morton) {
  uint64_t temp_val = morton;
  temp_val &= 0x5555555555555555;
  temp_val = (temp_val ^ (temp_val >> 1)) & 0x3333333333333333;
  temp_val = (temp_val ^ (temp_val >> 2)) & 0x0F0F0F0F0F0F0F0F;
  temp_val = (temp_val ^ (temp_val >> 4)) & 0x00FF00FF00FF00FF;
  temp_val = (temp_val ^ (temp_val >> 8)) & 0x0000FFFF0000FFFF;
  temp_val = (temp_val ^ (temp_val >> 16)) & 0x00000000FFFFFFFF;
  return static_cast<int>(temp_val);
}

void GetNdsCoordFromMorton(uint64_t morton, int* x, int* y) {
//   DCHECK(x != nullptr);
//   DCHECK(y != nullptr);
  *x = CompactBits(morton);
  *y = SignedBitRestore(CompactBits(morton >> 1));
}

void GetTileBorderOfNds(uint32_t tile_id, int* left, int* bottom, int* right, int* top) {
  const int level = GetPackedTileIdLevel(tile_id);
  uint64_t tile = (0xFFFFFFFF >> (31 - 2 * level)) & tile_id;
  uint64_t code = tile << (62 - 2 * level);

  int x = 0;
  int y = 0;
  GetNdsCoordFromMorton(code, &x, &y);
  int tile_width = GetTileWidth(level);
  *left = x;
  *bottom = y;
  *right = x + tile_width;
  *top = y + tile_width;
}

void GetTileCenterNds(uint32_t tile_id, int* nds_x, int* nds_y) {
//   DCHECK(nds_x != nullptr);
//   DCHECK(nds_y != nullptr);
  int left;
  int bottom;
  int right;
  int top;
  GetTileBorderOfNds(tile_id, &left, &bottom, &right, &top);
  // double?
  *nds_x = left + (right - left) / 2;
  *nds_y = bottom + (top - bottom) / 2;
}

double GetDegCoordFromNds(int nds_val) { return static_cast<double>(90.0 * nds_val / (1 << 30)); }

void GetTileCenter(uint32_t tile_id, double* lon, double* lat) {
//   DCHECK(lon != nullptr);
//   DCHECK(lat != nullptr);
  int x = 0;
  int y = 0;
  GetTileCenterNds(tile_id, &x, &y);
  *lat = GetDegCoordFromNds(y);
  *lon = GetDegCoordFromNds(x);
}

std::vector<uint32_t> GetTilesByRadius(int level, double lon, double lat, int tile_radius) {
//   DCHECK(lon >= -180.0 && lon <= 180.0);
//   DCHECK(lat >= -90.0 && lat <= 90.0);
  std::vector<uint32_t> tile_id_list;
  double resolution = GetMapResolution(level);
  // In order to avoid floating error, we need to use the tile center instead.
  double center_lon;
  double center_lat;
  GetTileCenter(GetTileId(level, lon, lat), &center_lon, &center_lat);
  for (int dx = -tile_radius; dx <= tile_radius; dx++) {
    for (int dy = -tile_radius; dy <= tile_radius; dy++) {
      double new_lon = center_lon + dx * resolution;
      double new_lat = center_lat + dy * resolution;
      tile_id_list.push_back(GetTileId(level, new_lon, new_lat));
    }
  }
  return tile_id_list;
}

int main() {
    // 1 5 3
    std::vector<uint32_t> tiles = GetTilesByRadius(13, 120.610496, 31.029859, 3);
    for (const auto tid: tiles) {
        std::cout << tid << "\n";
    }
    // 557028438
    // 557028444
    // 557028446
    // 557028468
    // 557028470
    // 557028476
    // 557028478
    // 557028439
    // 557028445
    // 557028447
    // 557028469
    // 557028471
    // 557028477
    // 557028479
    // 557028610
    // 557028616
    // 557028618
    // 557028640
    // 557028642
    // 557028648
    // 557028650
    // 557028611
    // 557028617
    // 557028619
    // 557028641
    // 557028643
    // 557028649
    // 557028651
    // 557028614
    // 557028620
    // 557028622
    // 557028644
    // 557028646
    // 557028652
    // 557028654
    // 557028615
    // 557028621
    // 557028623
    // 557028645
    // 557028647
    // 557028653
    // 557028655
    // 557028626
    // 557028632
    // 557028634
    // 557028656
    // 557028658
    // 557028664
    // 557028666

    uint32_t tid = GetTileId(13, 120.610496, 31.029859);
    std::cout << " get tileid: " << tid;
    // 557028641
}