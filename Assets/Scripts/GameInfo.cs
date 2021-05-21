using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameInfo : GenericSingleton<GameInfo>
{
    public int CharacterIndex { get; set; }

    public Dictionary<string, Conversation> conversationList;
    public Dictionary<string, NpcInfo> npcInfos;

    protected override void Awake()
    {
        base.Awake();
        npcInfos = new Dictionary<string, NpcInfo>();
        conversationList = new Dictionary<string, Conversation>();
    }
}
