#include <stdio.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "struct.h"


typedef struct {
  double weight;
  double length;
  int callback;
  const char *name;
} dragon;


int dragon_new(void *obj)
{
  dragon *d = (dragon*) obj;
  d->weight = 0.0;
  d->length = 0.0;
  return 0;
}

int dragon_del(void *obj)
{
  //  printf("cleaning up after a dragon\n");
  return 0;
}

int dragon_speak(lua_State *L)
{
  dragon *d = (dragon*) luaL_checkudata(L, 1, "dragon");
  printf("%s-dragon says ROAR!\n", d->name);
  lua_struct_pushmember(L, d, "dragon", "callback");
  return 1;
}

int luaopen_legend(lua_State *L)
{
  lua_newtable(L);

  lua_struct_member_t lua_dragon_members[] = {
    {"weight", offsetof(dragon, weight), LSTRUCT_DOUBLE},
    {"length", offsetof(dragon, length), LSTRUCT_DOUBLE},
    {"callback", offsetof(dragon, callback), LSTRUCT_OBJECT},
    {"name", offsetof(dragon, name), LSTRUCT_STRING},
    {NULL, 0, 0},
  };
  lua_struct_t lua_dragon = {
    .type_name = "dragon",
    .alloc_size = sizeof(dragon),
    .new = dragon_new,
    .del = dragon_del,
    .members = lua_dragon_members,
  };

  lua_struct_register(L, lua_dragon);

  lua_pushcfunction(L, dragon_speak);
  lua_setfield(L, -2, "speak");
  lua_pushstring(L, "Legend version 0.0.1");
  lua_setfield(L, -2, "version");

  return 1;
}


int main(int argc, char **argv)
{
  int n;
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  luaL_requiref(L, "struct", luaopen_struct, 0); lua_pop(L, 1);
  luaL_requiref(L, "legend", luaopen_legend, 0); lua_pop(L, 1);


  // Create the global `arg` table
  // ---------------------------------------------------------------------------
  lua_newtable(L);
  for (n=0; n<argc; ++n) {
    lua_pushstring(L, argv[n]);
    lua_rawseti(L, -2, n);
  }
  lua_setglobal(L, "arg");


  // Run the script
  // ---------------------------------------------------------------------------
  if (argc == 1) {
    printf("usage: main script.lua [arg1=val1 arg2=val2]\n");
  }
  else {
    char luacode[4096];
    snprintf(luacode, 4096, "\
    local f, err = loadfile('%s')\n					\
    if not f then\n							\
      print(err)\n							\
    else								\
      local success, msg = xpcall(f, debug.traceback)\n			\
      if not success then\n						\
         print(msg)\n							\
      end\n								\
    end\n", argv[1]);
    int err = luaL_dostring(L, luacode);
    if (err) {
      printf("%s\n", lua_tostring(L, -1));
    }
  }

  lua_close(L);

  return 0;
}
