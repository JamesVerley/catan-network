# Route definitions

## game session
### from server
| Move response | Enum id | data |
|---|---|---|
| move success | 0 | none |
| move failed | 1 | none |
| move failed | 2 | none |
| update board | 3 | array of hex structs |
| player ids | 7 | 

> ### Hex structs
> landtype enum
> 

### from client
| Move name | Enum id | data |
|---|---|---|
| join game | 0 | user id -- int<br/> |
| reload all | 1 | none |
| chat | 1 | message -- string<br/> |
| roll dice | 2 | none |
| build road | 3 | tileId -- int<br/>edgeId -- int<br/>resourceCardIds -- int<br/>|
| build settlement | 4 | tileId -- int<br/>cornerId -- int<br/>resourceCardIds -- int<br/> |
| build city | 5 | tileId -- int<br/>cornerId -- int<br/>resourceCardIds -- int<br/> |
| place robber | 6 | tileId -- int<br/> |
| play developmentcard | 7 | devcardId<br/> |

## Lobby
### from server
### from client

## user database
### from server
### from client
|  | Enum id | data |
|---|---|---|