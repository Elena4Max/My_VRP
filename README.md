# Task for Junior VRP Engineer Position, VanOnGo

## Зависимости

Jsoncpp

* `sudo apt-get install libjsoncpp-dev`
* `sudo ln -s /usr/include/jsoncpp/json/ /usr/include/json`

## Как использовать

* `git clone https://github.com/Elena4Max/My_VRP.git`
* `cd Out/`
* `cmake ../`
* `make`
* `./main`

## Бонус №1

* `cmake -DBONUS=bonus1 ../`
* `make`
* `./main`

<p align="center">
  <img align="center" src="https://github.com/Elena4Max/My_VRP/blob/master/Out/bonus1.gif" alt="demo"/>
</p>

## Бонус №2

* `cmake -DBONUS=bonus2 ../`
* `make`
* `./main`

<p align="center">
  <img align="center" src="https://github.com/Elena4Max/My_VRP/blob/master/Out/bonus2.gif" alt="demo"/>
</p>

## Дополнительно

Грузовик двигается из точки (х,у) в точки (х-1,у), (х,у-1), (х,у+1), (х+1,у).
В бонусах 1 и 2 грузовики обозначены 'V' красным цветом, места погрузки 'S' зеленым цветом, места разгрузки 'D' зеленым цветом.
Время погрузки и разгрузки входит в стоимость.
В программе сделана обработка ошибок про чтении json файла (например файл vrp_task12.json) и применении бонусов 1 или 2 при значении координат 'x' > 35, 'y' > 130 (мой размер экрана не позволяет протестировать наглядно с такими значаениями).
На каждой итерации выполняется пересчет матрицы стоимостей алгоритмом bfs и оценивание матрицы жадным алгоритмом, принимается решение какой грузовик в каком направлении поедет.
При тестировании выполнялся полный перебор вариантов и поиск оптимального вручную и описанным методом программно, результаты сравнивались. Оценка оптимальных вариантов в двух случаях совпадала.


