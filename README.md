# Conflict Map

<p>A 2D GIS map showing conflict data from the 1990s to 2026. Built in C++ using Qt and its various JSON and OpenGL additions and widgets. This application uses data from UCDP and ACLED linked below:</p>

1. <a href="https://ucdp.uu.se/">UCDP</a>
2. <a href="https://acleddata.com/">ACLED</a>

## Running

<p>Before running, the csv files in res/acled and res/ucdp must be converted into json files.</p>

<p>For each of the sources, run these commands</p>

<p>UCDP:</p>

```shell
cd res/ucdp
python3 csv_to_json.py
```

<p>ACLED:</p>

```shell
cd res/acled
python3 csv_to_json.py
```

*Be sure to have the pandas library before running these scripts. Otherwise, they will fail to convert the CSVs into JSON files.

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

std::string path = "<path>/res/acled/ACLED Data_2026-10-31_event_date_from_2026-01-01_event_date_to_2026-10-25.json";

std::vector<MarkerInstance> coordinates = ACLED::GetConflictPoints(path.c_str());
std::reverse(coordinates.begin(), coordinates.end());

acledMarker.SetInstances(coordinates);
```

Afterwards, build the application with <code>cmake</code> to run the app and see the map of archived conflict points.

```shell
cmake CMakeLists.txt
cmake --build .
./conflict_map
```

## Samples (UCDP data shown only)

### Sample October 30, 2026 - Europe + Middle East + Sahara + Central Asia

<img width="1192" height="774" alt="Image" src="https://github.com/user-attachments/assets/900df94b-e41f-4283-a6e1-6f250c169e34" />

--- 

### Example of Plotted Markers - October 31, 2026 (Ukraine)

<img width="1182" height="789" alt="Image" src="https://github.com/user-attachments/assets/d39f7972-8971-493c-8af5-81ab4bee83ef" />

--- 

### Colored Markers
<p>The colors indicate how recent or old each event is. The more red a marker is, the older the event and the more green a marker, the newer the event. Colors blend between the two depending on the years.</p>

<p>Image below shows southern Russia, the Caucasus, and the Middle East.</p>

<p>*UCDP data will show colors between red and green, and ACLED data will show colors between blue and purple.</p>

<img width="1189" height="782" alt="Image" src="https://github.com/user-attachments/assets/62c0fc37-2eae-41c9-a407-876e74997d72" />