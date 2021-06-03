using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TimeManager : MonoBehaviour
{
    public Transform sun;

    public int year;
    public int month;
    public int day;
    public int hour;
    public int min;

    public float speed = 1f;

    [SerializeField] private float time = 0f;
    private void Start()
    {
        year = 0;
        month = 1;
        day = 1;
        hour = 0;
        min = 0;

    }

    private void FixedUpdate()
    {
        time += Time.fixedDeltaTime * speed; // 0.02
        min = (int)time % 60;
        hour = ((int)time / 60) % 24;
        day = ((int)time / 60) / 24;
        sun.Rotate(Vector3.right * 0.0025f * speed); //0.02f
    }

}
