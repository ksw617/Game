using System;

[Serializable]
public class JsonData
{
    public Conversation[] conversation;
    public CharacterInfo[] characterInfo;
    public MapInfo[] mapInfo;
}

[Serializable]
public class Conversation
{
    public string key;
    public string talk;
    public int buttonCount;
    public string answer1;
    public string answer2;
    public string answer3;
    public string next1;
    public string next2;
    public string next3;
}

[Serializable]
public class CharacterInfo
{
    public string name;
    public int conversationIndex;
    public string weapon;
    public float searchRegion;
    public float moveSpeed;
    public string occupation;
    public string characterShape;

}

[Serializable]
public class MapInfo
{
    public string sceneName;
    public string npc_0;
    public string npc_1;
    public string npc_2;
    public string npc_3;
    public string npc_4;
}


