using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameInfo : GenericSingleton<GameInfo>
{
    public int CharacterIndex { get; set; }

    public Dictionary<string, Conversation> conversationList;
    public Dictionary<string, CharacterInfo> characterInfos;
    public Dictionary<string, int> characterNames;

    protected override void Awake()
    {
        base.Awake();
        characterInfos = new Dictionary<string, CharacterInfo>();
        conversationList = new Dictionary<string, Conversation>();
        characterNames = new Dictionary<string, int>();
    }
}
