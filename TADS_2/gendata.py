from random import randint, choice
import sys

out_filename = "data.txt"
N = 60 if len(sys.argv) == 1 else int(sys.argv[1])
data = []

AFRICA = 0
ANTARCTICA = 1
AUSTRALIA = 2
EURASIA = 3
NORTH_AMERICA = 4
SOUTH_AMERICA = 5

countries = [
    [ "Swaziland", 1160164, "Mbabane", AFRICA ],
    [ "Ghana", 31072940, "Accra", AFRICA ],
    [ "Macao", 667400, "Ciudad de Macao", EURASIA ],
    [ "Croatia", 4154213, "Zagreb", EURASIA ],
    [ "Ecuador", 17643054, "Quito", SOUTH_AMERICA ],
    [ "Bulgaria", 6951482, "Sofia", EURASIA ],
    [ "Nepal", 29136808, "Kathmandu", EURASIA ],
    [ "Sweden", 10377781, "Stockholm", EURASIA ],
    [ "Congo", 90945551, "Kinshasa", AFRICA ],
    [ "Mexico", 128932753, "Mexico City", SOUTH_AMERICA ],
    [ "Colombia", 50612648, "Bogota", SOUTH_AMERICA ],
    [ "Russia", 146748590, "Moscow", EURASIA ],
    [ "Canada", 37602103, "Ottawa", NORTH_AMERICA ]
]

SIGHTSEEING = 0
BEACH = 1
SPORT = 2

sightseeings = lambda: [
    SIGHTSEEING, randint(20, 80), randint(0, 2)
]

beach = lambda: [
    BEACH, randint(0, 2),
    round(randint(200, 280) / 10, 1),
    round(randint(180, 250) / 10, 1),
    round(randint(50, 150) / 10, 1)
]

sport = lambda: [
    SPORT, randint(0, 2), randint(10000, 100000)
]

for i in range(N):
    country = list(choice(countries))
    country.extend(choice([sightseeings, beach, sport])())
    data.append(country)

with open(out_filename, "wt") as file:
    for country in data:
        print(*country, sep=",", file=file)
