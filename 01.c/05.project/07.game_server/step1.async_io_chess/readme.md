---\
How 


# Create new game
`curl http://localhost:8080/new_game`

# Get game state (replace 1 with actual game_id)
`curl http://localhost:8080/game/1`

# Test invalid game
`curl http://localhost:8080/game/99999`


# Test with scenario
```
# 1. Create new game and save game_id
GAME_ID=$(curl -s http://localhost:8080/new_game | grep -o '"game_id":[0-9]*' | cut -d':' -f2)

# 2. Get game state
curl http://localhost:8080/game/$GAME_ID

# 3. Make a move (when implemented)
curl -X POST http://localhost:8080/move/$GAME_ID -d "move=e2e4"
```
