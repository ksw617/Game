using System;

[Serializable]
public class JsonData
{
    public Conversation[] conversation;
    public CharacterInfo[] characterInfo;
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


