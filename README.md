# Dokumentace hry Hangman

**Autor:** Kemyar Achilov  
**Předmět:** PCC, ČVUT FEL  
**Aktualizace dokumentace:** 10. listopadu 2026  
**Verze kódu:** `2629f23601802228371e253f534357215c49ea65`  
**Repozitář:** [Kemyar1/PCC](https://github.com/Kemyar1/PCC)

Hangman je terminálová hra pro jednoho hráče v C++23. Dokumentace popisuje pravidla, třídy a metody, sestavení, ovládání a ověření programu. Rozhraní hry je v angličtině.

## 1 Popis hry a herní strategie

Na začátku kola se vylosuje anglické slovo ze slovníku. Každé písmeno je skryté jednou hvězdičkou. Správný odhad odkryje všechny výskyty písmene: pro `level` změní `l` masku `*****` na `l***l`. Platné písmeno mimo slovo přidá jednu chybu a další část postavy.

Hráč vyhraje odkrytím celého neprázdného slova před šestou chybou. Při šesté chybě prohraje. Neplatný vstup a opakované písmeno počet chyb nezvyšují. Velká písmena se převádějí na malá, takže `L` a `l` jsou stejný pokus. Po výsledku se zobrazí hlavní nabídka.

**Strategie pro výhru:** začít samohláskami a podle délky a odkrytých pozic volit další písmena. Výhru lze při testování zaručit jednoslovným slovníkem `level` a vstupy `l, e, v`. Masky jsou `l***l`, `le*el` a `level`. **Strategie pro prohru:** zadat `a, b, c, d, f, g`, tedy šest různých písmen mimo toto slovo.

Remíza není součástí pravidel. Hra nemá časovač ani prohru vypršením času. Samotný konec vstupu EOF nepřiděluje výhru či prohru; poslední načtený platný znak se ještě vyhodnotí.

## 2 Postup řešení a implementace

`main.cpp` vytvoří HangmanGame se slovníkem `hangman_data/words.txt` a HangmanApp, do které předá hru. App vlastní TerminalUI přes `std::unique_ptr`, UI používá odkazy na hru a proudy a HangmanGame obsahuje WordBank. Návrh odděluje pravidla od terminálu [1].

| Část projektu | Odpovědnost |
| --- | --- |
| app / HangmanApp | Argumenty programu, hlavní nabídka a spouštění kol. |
| ui / TerminalUI | Vstup, zprávy, maska a sedm obrázků šibenice. |
| core / HangmanGame | Kontrola písmen, historie, počet chyb a výsledek kola. |
| core / WordBank | Načtení a výběr slova, maskování a odkrytí pozic. |
| core/library / Result | Hodnota nebo konkrétní chybový kód operace. |

Při Start zavolá UI resetGame, ověří načtení slovníku a opakuje kontrolu výsledku, výpis stavu a čtení pokusu. Aplikace je jednovláknová a používá řádkový vstup potvrzený Enterem. ANSI sekvence slouží k barvám; terminál se nepřepíná do raw režimu.

### Implementované metody jádra

#### HangmanGame

Uchovává WordBank, vektor `guessedLetters` a počítadlo `wrongAttempts`. Pravidla neprovádějí žádné terminálové výpisy.

| Metoda | Popis |
| --- | --- |
| `HangmanGame(dictionaryPath)` | Předá cestu do WordBank; konstruktor ještě nelosuje slovo. |
| `guessLetter(letter)` | Ověří a zapíše normalizovaný znak, odkryje výskyty. Nenalezené písmeno vrátí LetterNotFound a přidá chybu. |
| `checkLetter(letter)` | Vrátí úspěch, InvalidType pro neanglický znak nebo LetterWasUsed pro duplicitu. Stav hry nemění. |
| `openLetter(letter)` | Předá odkrytí shodných pozic do WordBank. |
| `createWord()` | Předá vytvoření slova do WordBank a vrátí GameInitStatus; historii ani počet chyb neresetuje. |
| `resetGame()` | Vymaže historii, nastaví 0 chyb a vytvoří nové slovo. |
| `isUserWin()` | Vrací Playing, Won po odkrytí slova nebo Lost od 6 chyb. Prázdné slovo není výhra. |
| `getEncryptedWord()`<br>`getGuessedWord()`<br>`getGuessedLetters()`<br>`getWrongAttempts()` | Postupně vracejí masku a tajné slovo jako konstantní reference, kopii historie a počet chyb. |

Soukromé `myIsAlpha(ch)` ověřuje ASCII A–Z a a–z, `normalizeLetter(letter)` převádí velká písmena na malá. `isLetterUsed(letter)` hledá v historii a `addUsedLetter(letter)` do ní přidává platný normalizovaný pokus.

#### WordBank

Uchovává cestu ke slovníku, tajné slovo a masku. Při výběru načte položky do vektoru; index losuje `std::uniform_int_distribution` s generátorem `std::mt19937` inicializovaným `std::random_device`.

| Metoda | Popis |
| --- | --- |
| `WordBank(filePath)` | Uloží cestu; slovo se vybere až při vytvoření kola. |
| `getWordCount()` | Počítá položky oddělené bílými znaky. Chybějící soubor: nullopt; prázdný soubor: 0. |
| `getRandomNumber()` | Vrací index 1 až N; pro chybějící nebo prázdný slovník vrací nullopt. |
| `chooseRandomWord()` | Vrací slovo převedené na malá písmena nebo RandomWorldError: FileNotOpen či IndexWordNotExist. |
| `createWord()` | Nastaví slovo a masku z hvězdiček. Vrací Success, FileNotOpen nebo IndexWordNotExist; při chybě slovo i masku vymaže. |
| `openLetter(userLetter)` | Odkryje všechny shodné pozice; tajné slovo nemění. |
| `encryptWord()` | Obnoví masku s jednou hvězdičkou na každý znak. |
| `getGuessedWord()`<br>`getEncryptedWord()` | Vracejí konstantní reference na tajné slovo a masku. |
| `setWord(word)` | Nastaví encryptedWord, tedy masku; nemění tajné slovo. |

### Metody aplikace a rozhraní

#### HangmanApp

Konstruktor `HangmanApp(game)` vytvoří UI. `run(argc, argv)` zpracuje --help a jinak opakuje nabídku; `printMainMenu()` vypíše Start a Exit. Soukromé statické `readInt(prompt)` načte číslo nebo vrátí optional bez hodnoty, při běžné chybě obnoví proud a přeskočí řádek. EOF ukončí aplikaci.

#### TerminalUI

Výchozí proudy jsou `std::cin` a `std::cout`. Testy předávají řetězcové proudy, takže lze ověřit vstup i text zpráv bez globálního přesměrování.

| Metoda | Popis |
| --- | --- |
| `TerminalUI(game, input, output)` | Uloží nevlastnící odkazy na hru a proudy. |
| `execute()` | Resetuje kolo, zpracuje chybu slovníku a řídí čtení a výpisy do výsledku nebo EOF. |
| `handleTypeLetter()` | Načte znak, zavolá guessLetter, vypíše zprávu a vrátí Result. |
| `handlePrintUserStatus()` | Zjistí UserStatus; při výhře či prohře vypíše výsledek a tajné slovo. |
| `handleViewWord()`<br>`handleViewWrongAttempts()` | Vypisují masku, respektive počet chyb a odpovídající obrázek. |
| `handlePrintHelp()` | Vypíše použití programu, pravidla a ovládání. |
| `handleCreateWord()` | Pomocně volá createWord; běžné nové kolo zahajuje execute přes resetGame. |
| `readChar(prompt)` | Soukromě čte celý řádek a přijímá právě jeden znak; jinak nullopt. EOF zachová v proudu. |
| `readInt(prompt)`<br>`readLine(prompt)` | Soukromé pomocné čtení čísla a řádku po přeskočení bílých znaků; v herní smyčce se nepoužívají. |
| `drawHangmanStage(mistakes)` | Vybere stupeň 0 pro ≤ 0, stupně 1 až 5 přímo, jinak stupeň 6. |
| `drawStage0()` až `drawStage6()` | Pořadí obrázků: prázdná šibenice, hlava, trup, první paže, druhá paže, první noha a druhá noha. |

`exitGame()` je pouze deklarována bez definice a aplikace ji nevolá.

#### Result a společné stavy

`Result<V, E>` obsahuje hodnotu nebo chybu přes `std::unique_ptr`. Soukromý konstruktor převezme ukazatele, `success(value)` a `failure(error)` vytvoří příslušnou variantu. `hasValue()` a `operator bool()` ověřují přítomnost hodnoty; i uložené false je úspěšný výsledek. `value()` a `error()` vracejí konstantní reference, při chybné variantě vyhodí `std::logic_error`. Přesun přenáší vlastnictví, kopírování je zakázáno.

`UserStatus` rozlišuje Playing, Won a Lost; `GameInitStatus` Success, FileNotOpen a IndexWordNotExist. `CheckingLetterError` obsahuje InvalidType a LetterWasUsed; `GuessLetterError` navíc LetterNotFound. Poslední stav přičítá chybu, ostatní dva stav hry nemění. `RandomWorldError` popisuje chybu otevření slovníku nebo výběru indexu.

## 3 Kompilace a spuštění

Požadavky: CMake alespoň 3.20, překladač s C++23 (GCC nebo Clang) a Make či Ninja. Catch2 je přiložen v tests/vendor [4]. Ověřeno v Linuxu s GCC 14.2.0 a CMake 3.31.10. Použijte novou sestavovací složku; cizí CMakeCache.txt se nepřebírá.

Z kořene projektu s main.cpp a CMakeLists.txt [2]:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel 2
./build/HangmanApp
```

Ve Windows lze použít Clang a Ninja dostupné v PATH nebo nastavený toolchain v CLionu. Příklad pro PowerShell:

```powershell
cmake -S . -B build-win -G Ninja `
  -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
cmake --build build-win --parallel 2
.\build-win\HangmanApp.exe
```

CMake sestaví HangmanCore, HangmanApp a HangmanTests a kopíruje hangman_data do build. Běžná hra hledá `hangman_data/words.txt` vůči pracovní složce; spouští se z kořene projektu nebo z build. Slovník obsahuje slova oddělená bílými znaky. Mají používat jen A–Z a a–z; obsah položek se samostatně nevaliduje.

## 4 Argumenty příkazové řádky

Přepínač `--help` vypíše nápovědu a skončí bez zahájení kola; nepotřebuje slovník. Bez argumentu se otevře nabídka. Přepínač zpracovává HangmanApp, nikoli CMake. Další přepínače nejsou implementovány a neznámé argumenty se ignorují.

- Linux: `./build/HangmanApp --help`
- PowerShell při sestavení do `build-win`: `.\build-win\HangmanApp.exe --help`

## 5 Ovládání aplikace

| Vstup | Účinek |
| --- | --- |
| 1 a Enter v nabídce | Spustí nové kolo; po výsledku lze zvolit 1 znovu. |
| 0 a Enter v nabídce | Ukončí aplikaci. |
| Jedno písmeno a Enter v kole | Vyhodnotí odhad; více znaků, prázdný řádek a interpunkci odmítne. |
| EOF při čtení | Ukončí čtení a aplikaci bez nekonečné smyčky. |

`q` se hádá jako běžné písmeno; 0 a 1 uvnitř kola nejsou příkazy. Nabídka načítá číselný prefix řádku a zbytek přeskočí. Správný odhad a YOU WIN! jsou zelené, chybné písmeno a GAME OVER červené, neplatný či opakovaný vstup oranžový a slovo azurové. Pro barvy je nutný ANSI terminál.

## 6 Testování programu

Catch2 2.13.10 ověřuje hodnoty, chybové kódy, masku, historii, počítadlo a texty UI. Dne 9. listopadu 2026 prošlo v uvedeném prostředí **57 testů s 346 kontrolami**, v CTestu 0 neúspěšných. Rozdělení: Result 8, WordBank 17, HangmanGame 18 a TerminalUI 14. Dalších devět scénářů celého procesu ověřilo výhru, prohru, vadný vstup, EOF a chyby slovníku.

Názvy mají tvar `JmenoMetody_TestovanyStav_OcekavanyVysledek`. Vstupy a očekávané hodnoty jsou lokální proměnné; skutečný výsledek se uloží před REQUIRE. Soukromé pomocné metody se ověřují přes veřejné chování.

```bash
cmake --build build --target HangmanTests --parallel 2
ctest --test-dir build --output-on-failure
```

**Přímé spuštění Catch2:** Linux `./build/HangmanTests --reporter compact`, PowerShell `.\build\HangmanTests.exe --reporter compact`. Při sestavení do `build-win` nahraďte cestu `build` odpovídající složkou.

CTest registruje testy přes catch_discover_tests a nastavuje pracovní složku projektu [3]. TestMain.cpp používá CATCH_CONFIG_MAIN. Pomocník `testDataPath(filename)` odvodí tests/data z umístění hlavičky přes `__FILE__` [5]. Po přesunu zdrojů se testy znovu sestaví. Limit 5 s v CTestu chrání před zablokováním testu, nejde o herní časovač.

### Testovací data

| Soubor v tests/data | Obsah a očekávání |
| --- | --- |
| words.txt / mixed_case.txt | level / LeVeL; výběr vrátí level a index 1. |
| multiple.txt | level apple\nbanana\tpear\n; 4 položky, index 1 až 4. |
| empty.txt / whitespace.txt | Prázdný obsah / jen bílé znaky; počet 0, nelze vytvořit kolo. |
| does_not_exist.txt | Záměrně neexistuje; FileNotOpen nebo nullopt podle metody. |

### Scénáře a očekávané výsledky

Pokud není uvedeno jinak, každý případ začíná novým kolem se slovem level. Zápis \n označuje nový řádek a \t tabulátor.

| Vstup nebo volání | Očekávaný výsledek |
| --- | --- |
| `guessLetter('l')` | Úspěch, maska `l***l`, 0 chyb, historie obsahuje l. |
| `guessLetter('a')` dvakrát | První LetterNotFound a 1 chyba; druhé LetterWasUsed, stále 1 chyba. |
| L, potom l | Odkryje `l***l`, potom duplicita bez další chyby. |
| 123, +, l e nebo prázdný řádek | InvalidType; maska, historie a počet chyb se nemění. |
| Prázdný řádek, pak l\n | První řádek neplatný; následující l zůstává k načtení. |
| l, e, v / 5 a 6 chybných písmen | Won a level / při 5 chybách Playing, při šesté Lost. |
| resetGame po odehraných pokusech | 0 chyb, prázdná historie, maska `*****` a stav Playing. |
| `openLetter('l')` / `encryptWord()` | Odkrytí obou l / maska obsahuje přesně 5 hvězdiček. |
| Výběr z multiple.txt | Index v rozsahu a slovo z množiny level, apple, banana, pear. |
| `Result::success(false)`; chybný accessor | Úspěšná varianta s hodnotou false; chybné value/error vyhodí logic_error. |

### Hraniční situace a reprodukce

Chybějící slovník při Start vypíše **ERROR: File was not opened**. Prázdný slovník i obsah tvořený bílými znaky vypíší **ERROR: Dictionary is empty**. Kolo nezačne a neohlásí výhru. Nápověda funguje i bez slovníku. EOF ukončí aplikaci; vstup `l\ne\nv` bez posledního nového řádku se přesto dokončí jako výhra.

Pro tyto scénáře v PowerShellu 5.1 i 7 nastavte $buildDir podle své složky build nebo build-win. Připraví se jednoslovný slovník. První spuštění vede na YOU WIN!, druhé na GAME OVER [8]. Příkazy zadejte z kořene projektu.

```powershell
$buildDir = "build"
$gamePath = (Resolve-Path "$buildDir\HangmanApp.exe").Path
New-Item -ItemType Directory -Force "$buildDir\manual\hangman_data" | Out-Null
Copy-Item ".\tests\data\words.txt" "$buildDir\manual\hangman_data\words.txt"
Push-Location "$buildDir\manual"
"1`nl`ne`nv`n0" | & $gamePath
"1`na`nb`nc`nd`nf`ng`n0" | & $gamePath
Pop-Location
```

Remíza a vypršení herního času se netestují jako herní výsledky, protože nejsou implementovány. Test losování kontroluje rozsah a příslušnost ke slovníku, nikoli statistickou rovnoměrnost. Procentní pokrytí kódu a analýza úniků paměti nejsou součástí uvedeného výsledku testů.

## 7 Videozáznamy

K semestrální práci se odevzdávají **dvě videa**. Videa se kvůli velikosti ukládají na **fakultní GitLab**; do BRUTE se přikládá kód, dokumentace a prezentace. V této dokumentaci budou přímé odkazy na videa a časy začátků testovacích scénářů.

### Video překladu programu

- Na začátku zobrazit **Kemyar Achilov** a název **Hangman**.
- Zaznamenat terminál s příkazy pro konfiguraci a překlad, jejich spuštění a **celý průběh sestavení**. Použít příkazy z kapitoly 3 pro zvolené prostředí.
- Pro soubor je vhodný název `build.mp4`. Zadání pro tento záznam samostatný časový limit neurčuje.

**Odkaz na video překladu:** DOPLNIT ODKAZ NA FAKULTNÍ GITLAB.

### Video hry a testování

- Na začátku uvést jméno autora a název projektu **písemně i slovně**.
- Ukázat ovládání a testovací scénáře: **výhru, prohru a hraniční situace**. Pro tuto hru jsou vhodné neplatné a opakované vstupy, EOF a chyba slovníku.
- Záznam musí být **ozvučený** a obsahovat komentář k postupu testování a výsledkům.
- Použít formát **MP4**, například `gameplay.mp4`. Maximální délka je **10 minut**; zadání připouští výjimku pro delší hry.

**Odkaz na video hry a testování:** DOPLNIT ODKAZ NA FAKULTNÍ GITLAB.

Časy se vyplní podle hotových záznamů. Remíza a vypršení herního limitu nejsou funkcemi této verze hry.

| Video | Začátek | Obsah |
| --- | --- | --- |
| Překlad | DOPLNIT mm:ss | Autor a název projektu, příkazy a úplný překlad. |
| Hra | DOPLNIT mm:ss | Písemné a slovní představení autora, spuštění a ovládání. |
| Hra | DOPLNIT mm:ss | Výhra. |
| Hra | DOPLNIT mm:ss | Prohra po šesti chybných písmenech. |
| Hra | DOPLNIT mm:ss | Neplatný a opakovaný vstup, případně EOF. |
| Hra | DOPLNIT mm:ss | Chyba slovníku a souhrnný výsledek automatických testů. |

## 8 Použité zdroje

[1] ACHILOV, Kemyar. Hangman, vlastní kód a testy. Verze kódu je uvedena v úvodu dokumentace. [https://github.com/Kemyar1/PCC](https://github.com/Kemyar1/PCC) [cit. 2026-11-09].

[2] KITWARE. CMake 3.31, cmake(1). Konfigurace a sestavení. [https://cmake.org/cmake/help/v3.31/manual/cmake.1.html](https://cmake.org/cmake/help/v3.31/manual/cmake.1.html) [cit. 2026-11-09].

[3] KITWARE. CMake 3.31, ctest(1). Spouštění testů. [https://cmake.org/cmake/help/v3.31/manual/ctest.1.html](https://cmake.org/cmake/help/v3.31/manual/ctest.1.html) [cit. 2026-11-09].

[4] CATCH2 CONTRIBUTORS. Catch2 2.13.10. Přiloženo v tests/vendor včetně LICENSE_Catch2.txt. [https://github.com/catchorg/Catch2/tree/v2.13.10](https://github.com/catchorg/Catch2/tree/v2.13.10) [cit. 2026-11-09].

[5] ISO C++ WORKING DRAFT. Predefined macro names, `__FILE__`. [https://eel.is/c++draft/cpp.predefined](https://eel.is/c++draft/cpp.predefined) [cit. 2026-11-09].

[6] OPENAI. ChatGPT. [https://chatgpt.com/](https://chatgpt.com/) [používáno během projektu, cit. 2026-11-09].

[7] Slovník hangman_data/words.txt. DOPLNIT původ, autora, odkaz a podmínky použití; u vlastního slovníku uvést vlastní sestavení.

[8] MICROSOFT. PowerShell, pravidla řetězců a operátory. [https://learn.microsoft.com/powershell/](https://learn.microsoft.com/powershell/) [cit. 2026-11-09].

**Použití AI [6]:** konzultace návrhu, vysvětlení C++, návrhy části testů a oprav, úpravy terminálových výpisů a příprava dokumentace. Za kontrolu výsledku a vysvětlení kódu odpovídá autor. Dokumentace se odevzdává jako `README.md` v kořenové složce projektu vedle `main.cpp`.
