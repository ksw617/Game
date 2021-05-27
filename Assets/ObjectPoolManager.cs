using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectPoolManager : MonoBehaviour
{
    public GameObject characterPrefab;
    public List<Character> characters;

    private void Awake()
    {
        characters = new List<Character>();
    }

    public Transform GetCharacter(string name)
    {
        if (GameInfo.Instance.characterInfos.ContainsKey(name))
        {
            CharacterInfo characterInfo = GameInfo.Instance.characterInfos[name];

            foreach (var character in characters)
            {
                if (!character.gameObject.activeSelf)
                {
                    character.SetCharacterInfo(characterInfo);
                    return character.transform;
                }
            }

            GameObject newObj = Instantiate(characterPrefab);
            newObj.SetActive(false);
            Character newCharacter = newObj.GetComponent<Character>();
            newCharacter.SetCharacterInfo(characterInfo);
            characters.Add(newCharacter);

            return newObj.transform;
        }
 

        return null;

    }

}
