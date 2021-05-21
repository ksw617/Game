using System;

[Serializable]
public class JsonData
{
    public Conversation[] conversation;
    public NpcInfo[] npc;
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
public class NpcInfo
{
    public string npcName;
    public int conversationIndex;
}


