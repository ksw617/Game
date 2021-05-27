using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameInfo : GenericSingleton<GameInfo>
{
    public int CharacterIndex { get; set; }

    //대화내용들을 담을 용도로
    public Dictionary<string, Conversation> conversationList;
    //캐릭터 정보들을 담을 용도로
    public Dictionary<string, CharacterInfo> characterInfos;
    //
    public Dictionary<string, int> characterNames;

    //dictionary들 초기화

    public Dictionary<string, string[]> mapNpcInfos;
    protected override void Awake()
    {
        base.Awake();
        characterInfos = new Dictionary<string, CharacterInfo>();
        conversationList = new Dictionary<string, Conversation>();
        characterNames = new Dictionary<string, int>();
        mapNpcInfos = new Dictionary<string, string[]>();
    }
}
