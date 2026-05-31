# Objektinio egzamino (išankstinė) užduotis

## Apie programą

Programa skirta tekstinio failo analizei. Nuskaito tekstą, valo žodžius (palaiko lietuviškas raides UTF-8), ir atlieka tris užduotis.

## Failų struktūra

```
cleanword.h / cleanword.cpp             — žodžių valymas ir UTF-8 apdorojimas
wordfrequency.h / wordfrequency.cpp     — žodžių dažnumo skaičiavimas
crossreference.h / crossreference.cpp   — kryžminių nuorodų lentelė
urls.h / urls.cpp                       — URL adresų paieška
main.cpp                                — pagrindinis failas
CMakeLists.txt                          — CMake konfigūracija
tekstas.txt                             — įvesties failas
url_end.txt                             — TLD sąrašas
```

## Programos funkcijos

- Žodžių dažnumo skaičiavimas
- Kryžminių nuorodų lentelės sudarymas
- URL adresų paieška pagal TLD sąrašą
- UTF-8 palaikymas (lietuviškos raidės, kirilica)

## Reikalavimai

- C++20 kompiliatorius
- Visual Studio 2022

## Instaliacija

1. Atsisiųsti arba klonuoti repozitoriją:
```bash
git clone https://github.com/USERNAME/asociatyvus-konteineriai.git
```

2. Atidaryti projekto **aplanką** Visual Studio 2022 per **File → Open → Folder**
3. VS automatiškai aptiks `CMakeLists.txt` ir sukonfigūruos projektą
4. Spausti **Build → Build All** (`Ctrl+Shift+B`)

## Paleidimas

1. Įdėti `tekstas.txt` ir `url_end.txt` į projekto aplanką
2. Sukompiliuoti ir paleisti

> **Pastaba:** su CMake programa paleidžiama iš `out\build\x64-Debug\` — įsitikinti kad ten yra `tekstas.txt` ir `url_end.txt`, arba pridėti į `CMakeLists.txt`:
> ```cmake
> configure_file(${CMAKE_SOURCE_DIR}/tekstas.txt ${CMAKE_BINARY_DIR}/tekstas.txt COPYONLY)
> configure_file(${CMAKE_SOURCE_DIR}/url_end.txt ${CMAKE_BINARY_DIR}/url_end.txt COPYONLY)
> ```

## Rezultatai

| Failas                    | Turinys                          |
|---------------------------|----------------------------------|
| `1_zodziu_daznumas.txt`   | Žodžiai pasikartojantys >1 kartą |
| `2_crossreference.txt`    | Žodžiai su eilučių numeriais     |
| `3_urls.txt`              | Rasti URL adresai                |
