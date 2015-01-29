# Client Protocol

`Client` could be `Oberserver` or `HumanPlayer` 

### Oberserver
Overserver will receive data from `Battle`, in the format of:

```
x_p1 y_p1 x_p2 y_p2
```
OR
```
END 
game info (e.g. Play1 wins! / Draw! / Time out!, etc)
```

and display the game status on screen
