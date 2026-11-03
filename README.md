# Conflict Map

<p>A 2D GIS map showing conflict data from the 1990s to 2026. Built in C++ using Qt and its various JSON and OpenGL additions and widgets. This application uses data from UCDP and ACLED linked below:</p>

1. <a href="https://ucdp.uu.se/">UCDP</a>
2. <a href="https://acleddata.com/">ACLED</a>

Additionally, all geojsons are from naturalearth.

## Running

<p>Before running, the csv files in res/ucdp must be converted into json files.</p>

<p>UCDP:</p>

```shell
cd res/ucdp
python3 csv_to_json.py
```

*Be sure to have the pandas library before running these scripts. Otherwise, they will fail to convert the CSVs into JSON files.

(ACLED data has alread been pre split)

<p>Additionally, in <code>src/widgets/MapRenderer.cpp</code>, the paths for loading geojsons and the conflict data have to be changed too.</p>

```c++
administrativeBorders = Borders();
administrativeBorders.Load("<path>/res/admin1.geojson");
administrativeBorders.Initialize();

internationalBorders = Borders();
internationalBorders.Load("<path>/res/world.geojson");
internationalBorders.Initialize();
```

```c++
for (int i = 0; i < 17; i++) {

    std::string path = "<path>/res/ucdp/GEDEvent_v26_1_" + std::to_string(i+1) +".json";

    std::vector<MarkerInstance> coordinates = UCDP::GetConflictPoints(path.c_str());

    for (MarkerInstance m : coordinates) {
        markers.push_back(m);
    }
}
```

```c++
acledMarker = Marker();
acledMarker.Initialize();

for (int i = 0; i < 3; i++) {
    std::string path = "<path>/res/acled/acled_data_" + std::to_string(i+1) + ".json";
    std::vector<MarkerInstance> coordinates = ACLED::GetConflictPoints(path.c_str());

    for (MarkerInstance m : coordinates) {
        acledMarkers.push_back(m);
    }
}
```

Afterwards, build the application with <code>cmake</code> to run the app and see the map of archived conflict points.

```shell
cmake CMakeLists.txt
cmake --build .
./conflict_map
```

## Samples

### Markers and Colored Markers
<p>The colors indicate how recent or old each event is. The more red a marker is, the older the event and the more green a marker, the newer the event. Colors blend between the two depending on the years.</p>

<p>Image below shows southern Russia, the Caucasus, and the Middle East.</p>

<p>*UCDP data will show colors between red and green, and ACLED data will show colors between blue and purple.</p>

---

### Sample Data over Afghanistan + Pakistan - November 1, 2026 (UCDP)

<img width="1510" height="907" alt="Image" src="https://github.com/user-attachments/assets/a2f6ad38-3e09-48d2-acc1-213d4c91448b" />

---

### Sample Data over Myanmar - November 2, 2026 (UCDP and ACLED)

<img width="1511" height="914" alt="Image" src="https://github.com/user-attachments/assets/c8d39f8f-f441-4ee8-90a9-0e42244e64c5" />

---

### Sample Data over the Sahel

<img width="1511" height="919" alt="Image" src="https://github.com/user-attachments/assets/761824b4-844b-4c0f-859b-6b16a9d5e270" />