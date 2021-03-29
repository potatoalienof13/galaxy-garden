This is program that generates cool looking wallpapers.

(If you want an image like the one I posted in the discord, you need to replace 
```
point_distances[i].distance = distance_between(xy, points[i].xy);
```
with
```
point_distances[i].distance = std::sin(distance_between(xy, points[i].xy)/40) + 1;
```
)
