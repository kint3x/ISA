# Popcl

Popcl je program, ktorý stiahne emaily protokolom POP3 zo serveru

## Spustenie programu

```bash
popcl <server> [-p <port>] [-T|-S [-c <certfile>] [-C <certaddr>]] [-d] [-n]\\ -a <auth_file> -o <out_dir>
```
<server> je povinný parameter, špecifikuje IP adresu alebo hostname serveru
<port> je port serveru, kde POP3 server počúva
-T je prepínač na šifrované prepojenie
-S je prepínač na nadviazanie nešifrovaného spojenia a potom prechod na šifrované
-d je prepínač, ktorý správu po stiahnutí zmaže
-n je prepínač, ktorý aktivuje sťahovanie iba nových správ
-a <auth_file>  je povinný parameter, a špecifikuje súbor s prihlasovacími údajmi vo formáte

```text
username=<meno>
password=<heslo>
```

!!! meno ani heslo nesmú obsahovať medzery, tie sú pri načítavaní odstránené

-o <out_dir> je povinný parameter, špecifikujúci zložku na ukladanie emailov



## Autor

Martin Matějka <xmatej55@stud.fit.vutbr.cz>
Brno University of Technology, Faculty of Information Technology 

